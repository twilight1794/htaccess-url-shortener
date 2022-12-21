#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

int main(void) {
    xmlDocPtr doc;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;
    xmlNodeSetPtr nodes;
    xmlChar *name, *uri;
    int size, i;

    xmlInitParser();

    doc = xmlReadFd(STDIN_FILENO, NULL, "utf-8", XML_PARSE_PEDANTIC);
    if (doc == NULL) {
        fprintf(stderr, "Error: unable to parse stream\n");
        return(-1);
    }
    fputs("Options +FollowSymLinks\nRewriteEngine on\nRewriteCond %{REQUEST_FILENAME} !-f\n", stdout);

    xpathCtx = xmlXPathNewContext(doc);
    if (xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc); 
        return(-1);
    }
    
    xpathObj = xmlXPathEvalExpression(BAD_CAST "//outline[@type=\"link\"]", xpathCtx);

    nodes = xpathObj->nodesetval;
    size = (nodes) ? nodes->nodeNr : 0;
    for (i = 0; i < size; i++) {
        name = xmlGetProp(nodes->nodeTab[i], BAD_CAST "text");
        uri = xmlGetProp(nodes->nodeTab[i], BAD_CAST "url");
        fprintf(stdout, "RewriteRule ^%s$ %s [R=307,QSA,NC,L]\n", name, uri);

        if (nodes->nodeTab[i]->type != XML_NAMESPACE_DECL) nodes->nodeTab[i] = NULL;
    }
    
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx); 
    
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}