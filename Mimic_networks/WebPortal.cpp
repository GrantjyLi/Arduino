#include"WebPortal.h"

namespace WebPortal{
    String getHTML(){
        return
        R"rawliteral(<!DOCTYPE html>
        <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width", initial-scale=1.0>
                <style>*{color: Black;size: 20px;}</style>
            </head>
            <body>
                <div id ="mainContainer">
                    <h1>Hi friend, try edit me!</h1>
                    <form id = "inputForm" action = "\submit" method = "post">
                        <label for="passwordEnter">Password: (30 characters max)</label><br>
                        <input type="text" class="inputBar" name="passwordEnter"><br><br>
                        <input type="submit" value="Enter">
                    </form>
                </div>
            </body>
        </html>)rawliteral";
    }

    void handleConnect(){
        Serial.println("Device connected.");
        server.send(200, "text/html", getHTML());

    }
} 
