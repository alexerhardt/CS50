<?php

    // configuration
    require("../includes/config.php");

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("register_form.php", ["title" => "Register"]);
    }

    // else if user reached page via POST (as by submitting a form via POST)
    else if ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        
        // Checks that all fields are entered, and that password matches confirmation
        if(empty($_POST["username"]))
        {
            apologize("Please provide a username");
        }
        else if(empty($_POST["password"]))
        {
            apologize("Please provide a password");
        }
        else if(empty($_POST["confirmation"]))
        {
            apologize("Please confirm your password");
        }
        else if($_POST["password"] != $_POST["confirmation"])
        {
            apologize("The passwords entered do not match. Please try again.");
        }
        
        // If registration fails, prompt user
        if( (CS50::query("INSERT IGNORE INTO users (username, hash, cash) VALUES(?, ?, 10000.0000)", $_POST["username"], password_hash($_POST["password"], PASSWORD_DEFAULT))) == 0)
        {
            apologize("User creation has failed. Please try a different username.");
        }
        // Else start session and log user in
        else
        {
            $rows = CS50::query("SELECT LAST_INSERT_ID() AS id");
            $id = $rows[0]["id"];
            $_SESSION["id"] = $id;
            redirect("/index.php");
        }
    }

?>