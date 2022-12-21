# static url shortener

This is a simple program to generate a valid .htaccess file that contain redirects, like an url shortener service. It only needs an OPML file as input, and the result will be sent to stdout. This does not generate html dummy files with an `http-equiv` meta tag, as other similar programs do.

For example, this outline:
```xml
<?xml version="1.0" encoding="utf-8" ?>
<opml>
  <body>
    <outline type="link" text="tv" url="https://www.wikidata.org/wiki/Wikidata:WikiProject_Movies/reports/TV_episodes/complete" />
    <outline type="link" text="prop" url="https://www.wikidata.org/wiki/Wikidata:Property_proposal/Overview" />
  </body>
</opml>
```
will generate this file:
```
Options +FollowSymLinks
RewriteEngine on
RewriteCond %{REQUEST_FILENAME} !-f
RewriteRule ^tv$ https://www.wikidata.org/wiki/Wikidata:WikiProject_Movies/reports/TV_episodes/complete [R=307,QSA,NC,L]
RewriteRule ^prop$ https://www.wikidata.org/wiki/Wikidata:Property_proposal/Overview [R=307,QSA,NC,L]
```
I plan to add support to other configuration files in the future.

## Compiling
### Unix
Just libxml2 headers are needed: e.g., on Debian, just install `libxml2-dev` package, and do `gcc redir.c -lxml2 -I /usr/include/libxml2` to compile it.

### Windows
I suppose using mingw-w64 inside some system like msys2 (for the xml library) should work, but I couldn't make work it.