<?php
    
    // configuration
    require("../includes/config.php"); 
    
    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // query db for sellable positions
        $pos_rows = CS50::query ("SELECT symbol FROM portfolios WHERE user_id = ?", $_SESSION["id"]);
        
        // if none exist, warn user
        if(empty($pos_rows))
        {
            apologize("Sorry, nothing to sell. Buy something first!");
        }
        // else create an array and render sell_form.php
        else
        {
            $sell_pos = [];
            foreach ($pos_rows as $position)
            {
                $sell_pos[] = [
                    "symbol" => $position["symbol"]
                ];
            }
            
            // render page
            render("sell_form.php", ["title" => "Sell Order", "positions" => $sell_pos]);
        }
    }
    
    // if user reached via POST
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // calculate value of share to be sold
        $stockinfo = lookup($_POST["symbol"]);
        $price = $stockinfo["price"];
        $share_row = CS50::query("SELECT shares FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        $shares = $share_row[0]["shares"];
        $sell_val = $price * $shares;
        
        // delete entry from portfolio
        $stock_del = CS50::query("DELETE FROM portfolios WHERE user_id = ? AND symbol = ?", $_SESSION["id"], $_POST["symbol"]);
        
        // warn if something went wrong
        if($stock_del === false)
        {
            apologize("Stock could not be sold. Please contact the person in charge.");
        }
        // otherwise update users db and send user back to index
        else
        {
            $update = CS50::query("UPDATE users SET cash = cash + ? WHERE id = ?", $sell_val, $_SESSION["id"]);
            // warn if there's an error
            if($update === false)
            {
                apologize("Your cash has not been credited. Please contact the person in charge");
            }
            // else log it and redirect back to index
            {
                $logit = history("SELL", $stockinfo["symbol"], $shares, $price);
                if($logit === false)
                {
                    apologize("Transaction has been processed correctly, but history update has failed. Please notify the person in charge.");
                }
                redirect("/");
            }
        }
    }

?>