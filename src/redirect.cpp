#include <Arduino.h>
String head = "<!DOCTYPE html>\n\
<html>\n\
  <head>\n\
    <meta http-equiv=\"refresh\" content=\"2; url='";

String tail = "'\"/>\n\
    <style>\n\
      body {\n\
        background-color: #f5f5f5;\n\
        font-family: 'Montserrat';\n\
        color: #333;\n\
        margin: 0;\n\
        padding: 0;\n\
      }\n\
      .container {\n\
        display: flex;\n\
        justify-content: center;\n\
        align-items: center;\n\
        height: 100vh;\n\
        text-align: center;\n\
      }\n\
      .box {\n\
        background-color: #e0e0e0;\n\
        border: 2px solid #555;\n\
        border-radius: 10px;\n\
        padding: 20px;\n\
        margin: 10px;\n\
      }\n\
      p {\n\
        font-size: 24px;\n\
        color: #555;\n\
        margin-top: 20px;\n\
      }\n\
    </style>\n\
  </head>\n\
  <body>\n\
    <div class=\"container\">\n\
      <div class=\"box\">\n\
        <p>You will be redirected soon! </p>\n\
      </div>\n\
    </div>\n\
  </body>\n\
</html>";
