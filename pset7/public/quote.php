<?php

    // Configuration
    require("../includes/config.php"); 

    // If user comes via GET
    if($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // Render quote form
        render("quote_form.php", ["title" => "Get A Quote"]);
    }

    // Else if user came via POST
    else if($_SERVER["REQUEST_METHOD"] == "POST")
    {
        if( ($stock = (lookup($_POST["symbol"]))) == false )
        {
        // Show error message if quote doesn't exist
        apologize("Could not find quote. Please try again.");
        }
        // Retrieve stock info, store in array
        else 
        {
        $stock = lookup($_POST["symbol"]);
        $stockinfo = [
        "symbol" => $stock["symbol"],
        "name" => $stock["name"],
        "price" => number_format($stock["price"], 2)
        ];
        render("quote_display.php", ["stockinfo" => $stockinfo, "title" => "Quote"]);
        }
    }
?>