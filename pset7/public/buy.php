<?php

    // configuration
    require("../includes/config.php"); 
    
    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        render("buy_form.php", ["title" => "Buy Order"]);
    }
    
    // if user reached via POST
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // check that there are no empty fields
        if( empty($_POST["shares"]) || empty($_POST["symbol"]) )
        {
            apologize("Please fill in all fields and try again.");
        }
        // lookup stock, if false, warn user
        $shares = $_POST["shares"];
        $stockinfo = lookup(strtoupper($_POST["symbol"]));
        if($stockinfo === false)
        {
            apologize("Stock not found. Please try again.");
        }
        // check that shares have been entered as a positive int
        $sharecheck = preg_match("/^\d+$/", $shares);
        if($sharecheck !== 1)
        {
            apologize("Number of shares can only be a positive integer. Please try again.");
        }
        
        // else carry on
        else
        {
            $price = $stockinfo["price"];
            $value = $shares * $price;
            
            // check if the user has enough cash to afford the operation
            $cash_row = CS50::query("SELECT cash FROM users WHERE id = ?", $_SESSION["id"]);
            if($cash_row === false)
            {
                apologize("Error retrieving current funds. Please contact the person in charge.");
            }
            $cash = $cash_row[0]["cash"];
            
            // warn user if they can't afford it
            if($cash < $value)
            {
                apologize("You cannot afford that. Please try again.");
            }
            // else if they can afford it
            else
            {
                // insert new position into portfolio
                $stock_insert = cs50::query("INSERT INTO portfolios (user_id, symbol, shares) VALUES (?, ?, ?) ON DUPLICATE KEY UPDATE shares = shares + ?", $_SESSION["id"], $stockinfo["symbol"], $shares, $shares);
                if($stock_insert === false)
                {
                    apologize("Error updating your stock positions. Please contact the person in charge.");
                }

                // subtract cash
                $cash_subtract = cs50::query("UPDATE users SET cash = cash - ? WHERE id = ?", $value, $_SESSION["id"]);
                if($cash_subtract === false)
                {
                    apologize("Error updating your cash balance. Please contact the person in charge.");
                }
                
                // log transaction in history
                $logit = history("BUY", $stockinfo["symbol"], $shares, $price);
                if($logit === false)
                {
                    apologize("Transaction has been processed correctly, but history update has failed. Please notify the person in charge.");
                }
                
                // redirect user back to portfolio
                redirect("/");
            }
        }
        
    }

?>
