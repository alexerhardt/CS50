<?php
    
    // configuration
    require("../includes/config.php"); 

    // query history db
    $rows = cs50::query("SELECT type, date_time, symbol, shares, price FROM history WHERE user_id = ?", $_SESSION["id"]);
    
    // build array with all entries for user
    $transactions = [];
    foreach($rows as $row)
    {
        $transactions[] = [
            "type" => $row["type"],
            "date_time" => $row["date_time"],
            "symbol" => $row["symbol"],
            "shares" => $row["shares"],
            "price" => number_format($row["price"], 2),
            "value" => number_format( ($row["price"] * $row["shares"]), 2 )
        ];
    }
    
    // render history passing array
    render("transactions.php", ["title" => "Transaction History", "transactions" => $transactions]);

?>