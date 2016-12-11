<?php

    // configuration
    require("../includes/config.php"); 

    // if user reached page via GET (as by clicking a link or via redirect)
    if ($_SERVER["REQUEST_METHOD"] == "GET")
    {
        // else render form
        render("account_settings.php", ["title" => "Account Settings"]);
    }
    
    // if user reached via POST
    elseif ($_SERVER["REQUEST_METHOD"] == "POST")
    {
        // store POST variables
        $oldpass = $_POST["old"];
        $newpass = $_POST["new"];
        $confirmation = $_POST["confirm"];
        
        // if any of the fields are empty
        if(empty($oldpass) || empty($newpass) || empty ($confirmation))
        {
            apologize("Please fill all fields and try again.");
        }
        
        // warn user if confirmation not entered correctly
        if($newpass !== $confirmation)
        {
            apologize("New passwords do not match. Please try again.");
        }
        
        // retrieve hash from database
        $passquery = CS50::query("SELECT * FROM users WHERE id = ?", $_SESSION["id"]);
        if($passquery === false)
        {
            apologize("There was an error retrieving your credentials. Please contact the person in charge");
        }
        $hash = $passquery[0]["hash"];
        
        // if passwords match
        if( password_verify($oldpass, $hash) )
        {
            $passupdate = CS50::query("UPDATE users SET hash = ? WHERE id = ?", password_hash($newpass, PASSWORD_DEFAULT), $_SESSION["id"]);
            if($passupdate !== false)
            {
                render("thanks.php", ["title" => "Thanks"]);
            }
        }
        // else if password verification failed
        else
        {
            apologize("Old password entered incorrectly. Please try again");
        }
    }