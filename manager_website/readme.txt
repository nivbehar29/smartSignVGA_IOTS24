how to launch the website:

    1) open terminal in "parking_manager.html" directory.
    2) run "python -m http.server <port>" ex. python -m http.server 800
    3) open browser and navigate to "http://localhost:<port>/"

for parking_manager-new:
you should have a folder "keys" which has a file called firebaseConfig.js
the file should contain the firebase config data like this:

export const firebaseConfig = {
    apiKey: "API_KEY",
    authDomain: "DOMAIN",
    databaseURL: "URL"
};

and you should replace API_KEY, DOMAIN, URL to you params