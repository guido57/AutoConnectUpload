// HTML root page. Redirect to /browse
static const char PAGE_ROOT[] PROGMEM = R"(
{
  "uri": "/",
  "title": "Tubular bells",
  "menu": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "value": "<h2>Tubular Bells</h2>"
    }
  ]
}
)";

// HTML Page to list, play, upload and delete files
static const char PAGE_BROWSE[] PROGMEM = R"(
{
  "uri": "/browse",
  "title": "Browse",
  "menu": true,
  "element": [
    {
      "name": "object",
      "type": "ACElement"
    }, 
    {
      "name": "play",
      "type": "ACSubmit",
      "value": "PLAY",
      "uri": "/play"
    },
    {
      "name": "delete",
      "type": "ACSubmit",
      "value": "DELETE",
      "uri": "/delete"
    },
    {
      "name": "upload",
      "type": "ACSubmit",
      "value": "UPLOAD FILE:",
      "uri": "/browse"
    },
    {
      "name": "upload_file",
      "type": "ACFile",
      "label": "",
      "store": "fs",
      "posterior": "br"
    },
    {
      "name": "radio",
      "type": "ACRadio",
      "value": [
      ],
      "label": "",
      "arrange": "vertical"
    }
  ]
}
)";


// HTML Page to play a mid file
static const char PAGE_PLAY[] PROGMEM = R"(
{
  "uri": "/play",
  "title": "play",
  "menu": false,
  "response": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "value": "<h2>play page</h2>"
    }
  ]
}
)";

// HTML Page to delete a file
static const char PAGE_DELETE[] PROGMEM = R"(
{
  "uri": "/delete",
  "title": "delete",
  "menu": false,
  "response": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "value": "<h2>delete page</h2>"
    }
  ]
}
)";
