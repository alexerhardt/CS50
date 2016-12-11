<?php

    // configuration
    require("../includes/config.php"); 
    
    // query positions owned by user
    $rows = CS50::query("SELECT symbol, shares FROM portfolios WHERE user_id = ?", $_SESSION["id"]);
    
    // if query was valid, iteratively build positions array with each stock owned by user
    $positions = [];
    $totalval = 0;
    if($rows !== false)
    {
        foreach($rows as $row)
        {
            $stockinfo = lookup($row["symbol"]);
            if($stockinfo !== false)
            {
                $positions[] = [
                    "symbol" => $row["symbol"],
                    "name" => $stockinfo["name"],
                    "shares" => number_format($row["shares"], 0),
                    "price" => number_format($stockinfo["price"], 2),
                    "value" => number_format(($row["shares"] * $stockinfo["price"]),2)
                ];
                // update total value of portfolio
                $totalval += ($row["shares"] * $stockinfo["price"]);
            }
        }
    }
    
    // query current cash balance from user and format it
    $cashquery = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
    $cash = $cashquery[0];
    $rcash = number_format($cash["cash"], 2);
    
    // calculate total value of portfolio and format it
    $totalval += $cash["cash"];
    $totalval = number_format($totalval, 2);

    // render portfolio
    render("portfolio.php", ["title" => "Portfolio", "positions" => $positions, "cash" => $rcash, "total" => $totalval]);

?>
