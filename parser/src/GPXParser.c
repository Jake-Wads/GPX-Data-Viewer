#include "GPXParser.h"
#include "LinkedListAPI.h"

//-----------------------------------------Helper Functions---------------------------------------------

void deleteGpxData(void* data) {
    if (data == NULL) {
        return;
    }
    GPXData* temp = (GPXData*)data;
    free(temp);
}

char* gpxDataToString( void* data) {
    if (data == NULL) {
        return NULL;
    }
    GPXData* temp = (GPXData*)data;
    char* string = (char*)malloc(sizeof(char)+1000);
    strcpy(string, "gpxData name: ");
    strcat(string, temp->name);
    strcat(string, ", value: ");
    strcat(string, temp->value);
    return string;
}
int compareGpxData(const void *first, const void *second) {
    return -1;
}

void deleteWaypoint(void* data) {
    if (data == NULL) {
        return;
    }
    Waypoint* temp = (Waypoint*)data;
    free(temp->name);
    freeList(temp->otherData);
    free(temp);
}

char* waypointToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    Waypoint* temp = (Waypoint*)data;
    char* string = (char*)malloc(sizeof(char)+1000);
    if (temp->name != NULL) {
        strcpy(string, "Waypoint name: ");
        strcat(string, temp->name);
        strcat(string, ", ");
    } else {
        strcpy(string, "Waypoint: ");
    }
    strcat(string, "Longitude: ");
    char num[256] = "";
    sprintf(num, "%lf", temp->longitude);
    strcat(string, num);
    strcat(string, ", Latitude: ");
    sprintf(num, "%lf", temp->latitude);
    strcat(string, num);
    strcat(string, ", otherData: ");
    char* s = toString(temp->otherData);
    strcat(string, s);
    free(s);

    return string;
}

int compareWaypoints(const void *first, const void *second) {
    return -1;
}

void deleteRoute(void* data) {
    if (data == NULL) {
        return;
    }
    Route* temp = (Route*)data;
    free(temp->name);
    freeList(temp->waypoints);
    freeList(temp->otherData);
    free(temp);
}
char* routeToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    Route* temp = (Route*)data;
    char* string = (char*)malloc(sizeof(char)*5000);
    strcpy(string, "Route name: ");
    strcat(string, temp->name);
    strcat(string, ", waypoints: ");
    char* s = toString(temp->waypoints);
    strcat(string, s);
    strcat(string, ", otherData: ");
    free(s);
    s = toString(temp->otherData);
    strcat(string, s);
    free(s);
    return string;

}

int compareRoutes(const void *first, const void *second) {
    return -1;
}

void deleteTrackSegment(void* data) {
    if (data == NULL) {
        return;
    }
    TrackSegment* temp = (TrackSegment*)data;
    freeList(temp->waypoints);
    free(temp);
}
char* trackSegmentToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    TrackSegment* temp = (TrackSegment*)data;
    char* string = (char*)malloc(sizeof(char)+1000);
    strcpy(string, "Track Segment waypoints: ");
    char* s = toString(temp->waypoints);
    strcat(string, s);
    free(s);
    return string;
}

int compareTrackSegments(const void *first, const void *second) {
    return -1;
}

void deleteTrack(void* data) {
    if (data == NULL) {
        return;
    }
    Track* temp = (Track*)data;
    free(temp->name);
    freeList(temp->segments);
    freeList(temp->otherData);
    free(temp);
}
char* trackToString(void* data) {
    if (data == NULL) {
        return NULL;
    }
    Track* temp = (Track*)data;
    char* string = (char*)malloc(sizeof(char)+1000);
    strcpy(string, "Track name: ");
    strcat(string, (char*)temp->name);
    strcat(string, ", Segments: ");
    char* s = (char*)toString(temp->segments);
    strcat(string, s);
    free(s);
    strcat(string, ", otherData: ");
    s = (char*)toString(temp->otherData);
    strcat(string, s);
    free(s);
    return string;
}

int compareTracks(const void *first, const void *second) {
    return -1;
}

//-----------------------------------------Getter Functions---------------------------------------------

//Total number of waypoints in the GPX file
int getNumWaypoints(const GPXdoc* doc) {
    if (doc == NULL) {
        return 0;
    }
    ListIterator iter = createIterator(doc->waypoints);
    int count = 0;
    void* elem;
	while((elem = nextElement(&iter)) != NULL){
		count++;
	}
    return count;
}

//Total number of routes in the GPX file
int getNumRoutes(const GPXdoc* doc) {
    if (doc == NULL) {
        return 0;
    }
    ListIterator iter = createIterator(doc->routes);
    int count = 0;
    void* elem;
	while((elem = nextElement(&iter)) != NULL){
		count++;
	}
    return count;
}

//Total number of tracks in the GPX file
int getNumTracks(const GPXdoc* doc) {
    if (doc == NULL) {
        return 0;
    }
    ListIterator iter = createIterator(doc->tracks);
    int count = 0;
    void* elem;
	while((elem = nextElement(&iter)) != NULL) {
		count++;
	}
    return count;
}

//Total number of segments in all tracks in the document
int getNumSegments(const GPXdoc* doc) {
    if (doc == NULL) {
        return 0;
    }
    int count = 0;
    for (Node* temp = (Node*)doc->tracks->head; temp != NULL; temp = temp->next) {
        count += getLength(((Track*)temp->data)->segments);
    }
    return count;
}

//Total number of GPXData elements in the document
int getNumGPXData(const GPXdoc* doc) {
    if (doc == NULL) {
        return 0;
    }
    int count = 0;

    ListIterator iter = createIterator(doc->waypoints);
    Waypoint* elem;
	while((elem = (Waypoint*)nextElement(&iter)) != NULL){
		if (elem->name != NULL) {
            count++;
        }
        count += getLength(elem->otherData);
	}

    iter = createIterator(doc->tracks);
    Track* elem2;
	while((elem2 = (Track*)nextElement(&iter)) != NULL){
		if (elem2->name != NULL) {
            count++;
        }
        count += getLength(elem2->otherData);
        ListIterator iter2 = createIterator(elem2->segments);
        TrackSegment* elem3;
	    while((elem3 = (TrackSegment*)nextElement(&iter2)) != NULL){
		    ListIterator iter3 = createIterator(elem3->waypoints);
            Waypoint* elem4;
	        while((elem4 = nextElement(&iter3)) != NULL){
		        if (elem4->name != NULL) {
                    count++;
                }
                count += getLength(elem4->otherData);
	        }
	    }
	}

    iter = createIterator(doc->routes);
    Route* elem5;
	while((elem5 = (Route*)nextElement(&iter)) != NULL){
		if (elem5->name != NULL) {
            count++;
        }
        count += getLength(elem5->otherData);
        ListIterator iter4 = createIterator(elem5->waypoints);
        Waypoint* elem6;
	    while((elem6 = nextElement(&iter4)) != NULL){
		    if (elem6->name != NULL) {
                count++;
            }
            count += getLength(elem6->otherData);
	    }
    }

    return count;
}

// Function that returns a waypoint with the given name.  If more than one exists, return the first one.
// Return NULL if the waypoint does not exist
Waypoint* getWaypoint(const GPXdoc* doc, char* name) {
    if (doc == NULL || name == NULL) {
        return NULL;
    }
    ListIterator iter = createIterator(doc->waypoints);
    Waypoint* elem;
	while((elem = (Waypoint*)nextElement(&iter)) != NULL){
		if (strcmp(elem->name, name) == 0) {
            return elem;
        }
	}
    return NULL;
}
// Function that returns a track with the given name.  If more than one exists, return the first one.
// Return NULL if the track does not exist
Track* getTrack(const GPXdoc* doc, char* name) {
    if (doc == NULL || name == NULL) {
        return NULL;
    }
    ListIterator iter = createIterator(doc->tracks);
    Track* elem;
	while((elem = (Track*)nextElement(&iter)) != NULL){
		if (strcmp(elem->name, name) == 0) {
            return elem;
        }
	}
    return NULL;
}
// Function that returns a route with the given name.  If more than one exists, return the first one.
// Return NULL if the route does not exist
Route* getRoute(const GPXdoc* doc, char* name) {
    if (doc == NULL || name == NULL) {
        return NULL;
    }
    ListIterator iter = createIterator(doc->routes);
    Route* elem;
	while((elem = (Route*)nextElement(&iter)) != NULL){
		if (strcmp(elem->name, name) == 0) {
            return elem;
        }
	}
    return NULL;
}

//-----------------------------------------Main Functions---------------------------------------------

char* GPXdocToString(GPXdoc* doc) {
    char* string = (char*)malloc(sizeof(char)+10000);
    strcpy(string, "GPXDoc creator: ");
    strcat(string, doc->creator);
    strcat(string, ("\nVersion: "));
    char s[10] = "";
    sprintf(s, "%lf", doc->version);
    strcat(string, s);
    strcat(string, "\nWaypoints: ");
    char* temp = (char*)toString(doc->waypoints);
    strcat(string, temp);
    free(temp);
    strcat(string, "\nRoutes: ");
    temp = (char*)toString(doc->routes);
    strcat(string, temp);
    free(temp);
    strcat(string, "\nTracks: ");
    temp = (char*)toString(doc->tracks);
    strcat(string, temp);
    free(temp);
    return string;
}

void deleteGPXdoc(GPXdoc* doc) {
    if (doc == NULL) {
        return;
    }
    xmlCleanupParser();
    free(doc->creator);
    freeList(doc->waypoints);
    freeList(doc->routes);
    freeList(doc->tracks);
    free(doc);
}

Waypoint* parseWpt(xmlNode *node) {

    xmlNode *cur_node = node;
    Waypoint *wayptPtr = (Waypoint*)malloc(sizeof(Waypoint));
    wayptPtr->name = NULL;

    // Iterate through every attribute of the current node
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if (strcmp(attrName, "lat") == 0) {
            double d;
            sscanf(cont, "%lf", &d);
            wayptPtr->latitude = d;
        } else if (strcmp(attrName, "lon") == 0) {
            double d;
            sscanf(cont, "%lf", &d);
            wayptPtr->longitude = d;
        }

        //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
    }

    void (*delPtr)(void*) = &deleteGpxData;
    char* (*strPtr)(void*) = &gpxDataToString;
    int (*cmpPtr)(const void*, const void*) = &compareGpxData;
    wayptPtr->otherData = initializeList(strPtr, delPtr, cmpPtr);

    for (xmlNode *child = cur_node->children; child != NULL; child = child->next) {
        if (strcmp((char*)child->name, "name") == 0) {
            char *cont = (char *)(xmlNodeGetContent(child));
            wayptPtr->name = cont;
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, cont);
        } else if (strcmp((char*)child->name, "text") != 0) {
            char *cont = (char *)(xmlNodeGetContent(child));
            GPXData* data = (GPXData*)malloc(sizeof(GPXData)+strlen(cont)+1);
            strcpy(data->name, (const char*)child->name);
            strcpy(data->value, cont);
            free(cont);
            insertBack(wayptPtr->otherData, data);
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, cont);
        }
    }

    return wayptPtr;

}

Route* parseRte(xmlNode *node) {

    xmlNode *cur_node = node;
    Route *rtePtr = (Route*)malloc(sizeof(Route));

    void (*delPtr)(void*) = &deleteWaypoint;
    char* (*strPtr)(void*) = &waypointToString;
    int (*cmpPtr)(const void*, const void*) = &compareWaypoints;
    rtePtr->waypoints = initializeList(strPtr, delPtr, cmpPtr);

    void (*delPtr2)(void*) = &deleteGpxData;
    char* (*strPtr2)(void*) = &gpxDataToString;
    int (*cmpPtr2)(const void*, const void*) = &compareGpxData;
    rtePtr->otherData = initializeList(strPtr2, delPtr2, cmpPtr2);

    for (xmlNode *child = cur_node->children; child != NULL; child = child->next) {

        if (strcmp((char*)child->name, "name") == 0) {
            char *nameString = (char *)(xmlNodeGetContent(child));
            rtePtr->name = nameString;
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, nameString);
        } else if (strcmp((char*)child->name, "rtept") == 0) {
            insertBack(rtePtr->waypoints, parseWpt(child));
        } else if (strcmp((char*)child->name, "text") != 0) {
            char *cont = (char *)(xmlNodeGetContent(child));
            GPXData* data = (GPXData*)malloc(sizeof(GPXData)+strlen(cont)+1);
            strcpy(data->name, (const char*)child->name);
            strcpy(data->value, cont);
            insertBack(rtePtr->otherData, data);
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, cont);
            free(cont);
        }
    }
    return rtePtr;
}

Track* parseTrk(xmlNode *node) {

    xmlNode *cur_node = node;
    Track *trkPtr = (Track*)malloc(sizeof(Track));

    void (*delPtr)(void*) = &deleteTrackSegment;
    char* (*strPtr)(void*) = &trackSegmentToString;
    int (*cmpPtr)(const void*, const void*) = &compareTrackSegments;

    void (*delPtr2)(void*) = &deleteGpxData;
    char* (*strPtr2)(void*) = &gpxDataToString;
    int (*cmpPtr2)(const void*, const void*) = &compareGpxData;

    trkPtr->segments = initializeList(strPtr, delPtr, cmpPtr);
    trkPtr->otherData = initializeList(strPtr2, delPtr2, cmpPtr2);

    for (xmlNode *child = cur_node->children; child != NULL; child = child->next) {

        if (strcmp((char*)child->name, "name") == 0) {
            char *nameString = (char *)(xmlNodeGetContent(child));
            trkPtr->name = nameString;
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, nameString);

        } else if (strcmp((char*)child->name, "trkseg") == 0) {

            TrackSegment* temp = (TrackSegment*)malloc(sizeof(TrackSegment));
            void (*delPtr)(void*) = &deleteWaypoint;
            char* (*strPtr)(void*) = &waypointToString;
            int (*cmpPtr)(const void*, const void*) = &compareWaypoints;
            temp->waypoints = initializeList(strPtr, delPtr, cmpPtr);

            for (xmlNode *child2 = child->children; child2 != NULL; child2 = child2->next) {
                if (strcmp((char*)child2->name, "trkpt") == 0) {
                    insertBack(temp->waypoints, parseWpt(child2));
                }
            }
            insertBack(trkPtr->segments, temp);

        } else if (strcmp((char*)child->name, "text") != 0) {
            char *cont = (char *)(xmlNodeGetContent(child));
            GPXData* data = (GPXData*)malloc(sizeof(GPXData)+strlen(cont)+1);
            strcpy(data->name, (const char*)child->name);
            strcpy(data->value, cont);
            insertBack(trkPtr->otherData, data);
            //printf("\tnode name: %s, node value = %s\n", (const char*)child->name, cont);
            free(cont);
        }
    }

    return trkPtr;
}

void parseGpx(xmlNode *node, GPXdoc *document) {
    xmlNode *cur_node = NULL;

    strcpy(document->namespace, (char *)node->ns->href);
    

    for (cur_node = node; cur_node != NULL; cur_node = cur_node->next) {

        // Iterate through every attribute of the current node
        xmlAttr *attr;
        for (attr = cur_node->properties; attr != NULL; attr = attr->next) {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);

            if (strcmp(attrName, "version") == 0) {
                double d;
                sscanf(cont, "%lf", &d);
                document->version = d;
            } else if (strcmp(attrName, "creator") == 0) {
                document->creator = malloc(strlen(cont)+3);
                strcpy(document->creator, cont);
            }

            //printf("\tattribute name: %s, attribute value = %s\n", attrName, cont);
        }
    }
}

void parseGpxDoc(xmlNode *node, GPXdoc *document, List* waypointList, List* routeList, List* trackList) {
    xmlNode *cur_node = NULL;

    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        //printf("GPXDOC Node: %s\n", cur_node->name);
        if (strcmp((char*)cur_node->name, "gpx") == 0) {
            parseGpx(cur_node, document);
        } else if (strcmp((char*)cur_node->name, "wpt") == 0) {
            insertBack(waypointList, parseWpt(cur_node));
        } else if (strcmp((char*)cur_node->name, "rte") == 0) {
            insertBack(routeList, parseRte(cur_node));
        } else if (strcmp((char*)cur_node->name, "trk") == 0) {
            insertBack(trackList, parseTrk(cur_node));
        }

        parseGpxDoc(cur_node->children, document, waypointList, routeList, trackList);
    }



}

GPXdoc* createGPXdoc(char* fileName) {

    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    LIBXML_TEST_VERSION

    doc = xmlReadFile(fileName, NULL, 0);

    if (doc == NULL) {
        printf("error: could not parse file %s\n", fileName);
        return NULL;
    }

    root_element = xmlDocGetRootElement(doc);

    GPXdoc* document = (GPXdoc*)malloc(sizeof(GPXdoc));

    //initialize the lists
    void (*delPtr)(void*) = &deleteWaypoint;
    char* (*strPtr)(void*) = &waypointToString;
    int (*cmpPtr)(const void*, const void*) = &compareWaypoints;

    void (*delPtr2)(void*) = &deleteRoute;
    char* (*strPtr2)(void*) = &routeToString;
    int (*cmpPtr2)(const void*, const void*) = &compareRoutes;


    void (*delPtr3)(void*) = &deleteTrack;
    char* (*strPtr3)(void*) = &trackToString;
    int (*cmpPtr3)(const void*, const void*) = &compareTracks;

    List* waypointList = initializeList(strPtr, delPtr, cmpPtr);
    List* routeList = initializeList(strPtr2, delPtr2, cmpPtr2);
    List* trackList = initializeList(strPtr3, delPtr3, cmpPtr3);


    parseGpxDoc(root_element, document, waypointList, routeList, trackList);
    document->waypoints = waypointList;
    document->routes = routeList;
    document->tracks = trackList;

    xmlFreeDoc(doc);
    xmlCleanupParser();

    return document;


}

//-----------------------------------------A2 Module 1 Functions---------------------------------------------

xmlDoc* gpxDocToXmlDoc(GPXdoc* data) {
    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL, node1 = NULL, node2 = NULL;/* node pointers */
    //char buff[256];
    int i, j, k, q;

    LIBXML_TEST_VERSION;

    /* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "gpx");
    char* s = malloc(1000);
    sprintf(s, "%.1lf", data->version);
    xmlNewProp(root_node, BAD_CAST "version", BAD_CAST s);
    xmlNewProp(root_node, BAD_CAST "creator", BAD_CAST data->creator);
    xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST data->namespace, NULL);
    xmlSetNs(root_node, ns);
    xmlDocSetRootElement(doc, root_node);

    //add data from gpxDoc to xml file
    //waypoints
    Node* listNode = data->waypoints->head;
    Node* listNode1;
    for (i = 0; i < data->waypoints->length; i++) {
        node = xmlNewChild(root_node, NULL, BAD_CAST "wpt", NULL);
        Waypoint* wpt = listNode->data;
        char s[10];
        sprintf(s, "%.6f", wpt->latitude);
        xmlNewProp(node, BAD_CAST "lat", BAD_CAST s);
        sprintf(s, "%.6f", wpt->longitude);
        xmlNewProp(node, BAD_CAST "lon", BAD_CAST s);
        if (strcmp(wpt->name, "") != 0) {
            xmlNewChild(node, NULL, BAD_CAST "name", (const xmlChar *)wpt->name);
        }
        listNode1 = wpt->otherData->head;
        for (j = 0; j < wpt->otherData->length; j++) {
            GPXData* d = listNode1->data;
            if (d->name != NULL && d->value != NULL) {
                xmlNewChild(node, NULL, BAD_CAST d->name, (const xmlChar *)d->value);
            }
            listNode1 = listNode1->next;
        }
        listNode = listNode->next;
    }
    //routes
    listNode = data->routes->head;
    Node* listNode2;
    for (i = 0; i < data->routes->length; i++) {
        node = xmlNewChild(root_node, NULL, BAD_CAST "rte", NULL);
        Route* rte = listNode->data;
        xmlNewChild(node, NULL, BAD_CAST "name", (const xmlChar *)rte->name);
        listNode1 = rte->otherData->head;
        for (j = 0; j < rte->otherData->length; j++) {
            GPXData* d = listNode1->data;
            if (d->name != NULL && d->value != NULL) {
                xmlNewChild(node, NULL, BAD_CAST d->name, (const xmlChar *)d->value);
            }
            listNode1 = listNode1->next;
        }
        listNode1 = rte->waypoints->head;
        for (j = 0; j < rte->waypoints->length; j++) {
            node1 = xmlNewChild(node, NULL, BAD_CAST "rtept", NULL);
            Waypoint* wpt = listNode1->data;
            char s[10];
            sprintf(s, "%.6f", wpt->latitude);
            xmlNewProp(node1, BAD_CAST "lat", BAD_CAST s);
            sprintf(s, "%.6f", wpt->longitude);
            xmlNewProp(node1, BAD_CAST "lon", BAD_CAST s);
            if (wpt->name != NULL) {
                xmlNewChild(node1, NULL, BAD_CAST "name", (const xmlChar *)wpt->name);
            }
            listNode2 = wpt->otherData->head;
            for (k = 0; k < wpt->otherData->length; k++) {
                GPXData* d = listNode2->data;
                if (d->name != NULL && d->value != NULL) {
                    xmlNewChild(node1, NULL, BAD_CAST d->name, (const xmlChar *)d->value);
                }
                listNode2 = listNode2->next;
            }
            listNode1 = listNode1->next;
        }
        listNode = listNode->next;
    }
    //tracks
    listNode = data->tracks->head;
    Node* listNode3;
    for (i = 0; i < data->tracks->length; i++) {
        node = xmlNewChild(root_node, NULL, BAD_CAST "trk", NULL);
        Track* trk = listNode->data;
        xmlNewChild(node, NULL, BAD_CAST "name", (const xmlChar *)trk->name);
        listNode1 = trk->otherData->head;
        for (j = 0; j < trk->otherData->length; j++) {
            GPXData* d = listNode1->data;
            if (d->name != NULL && d->value != NULL) {
                xmlNewChild(node, NULL, BAD_CAST d->name, (const xmlChar *)d->value);
            }
            listNode1 = listNode1->next;
        }
        listNode1 = trk->segments->head;
        for (j = 0; j < trk->segments->length; j++) {
            node1 = xmlNewChild(node, NULL, BAD_CAST "trkseg", NULL);
            TrackSegment* ts = listNode1->data;
            listNode2 = ts->waypoints->head;
            for (k = 0; k < ts->waypoints->length; k++) {
                node2 = xmlNewChild(node1, NULL, BAD_CAST "trkpt", NULL);
                Waypoint* wpt = listNode2->data;
                char s[10];
                sprintf(s, "%.6f", wpt->latitude);
                xmlNewProp(node2, BAD_CAST "lat", BAD_CAST s);
                sprintf(s, "%.6f", wpt->longitude);
                xmlNewProp(node2, BAD_CAST "lon", BAD_CAST s);
                if (wpt->name != NULL) {
                    xmlNewChild(node2, NULL, BAD_CAST "name", (const xmlChar *)wpt->name);
                }
                listNode3 = wpt->otherData->head;
                for (q = 0; q < wpt->otherData->length; q++) {
                    GPXData* d = listNode3->data;
                    if (d->name != NULL && d->value != NULL) {
                        xmlNewChild(node2, NULL, BAD_CAST d->name, (const xmlChar *)d->value);
                    }
                    listNode3 = listNode3->next;
                }
                listNode2 = listNode2->next;
            }
            listNode1 = listNode1->next;
        }
        listNode = listNode->next;
    }

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();

    return doc;
}

int validateXmlTree(xmlDocPtr doc, char* schemaFilename) {
    int validFlag;

	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	char *XSDFileName = schemaFilename;

    xmlLineNumbersDefault(1);

    ctxt = xmlSchemaNewParserCtxt(XSDFileName);

    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);
    //xmlSchemaDump(stdout, schema); //To print schema dump

	if (doc == NULL) {
		fprintf(stderr, "Could not parse\n");
	} else {
		xmlSchemaValidCtxtPtr ctxt;
		int ret;

		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);
		if (ret == 0) {
            validFlag = 1;
		} else if (ret > 0) {
            validFlag = 0;
		} else {
            validFlag = -1;
		}
		xmlSchemaFreeValidCtxt(ctxt);
		xmlFreeDoc(doc);
	}

	// free the resource
    if(schema != NULL) {
    	xmlSchemaFree(schema);
    }

    xmlSchemaCleanupTypes();
    xmlMemoryDump();

    return validFlag;
}

GPXdoc* createValidGPXdoc(char* fileName, char* gpxSchemaFile) {
    if (fileName == NULL || gpxSchemaFile == NULL) {
        return NULL;
    }
    GPXdoc* d = createGPXdoc(fileName);
    xmlDocPtr docXML = (xmlDocPtr)gpxDocToXmlDoc(d);
    int ans = validateXmlTree(docXML, gpxSchemaFile);
    if (ans == 1) {
        return d;
    } else {
        return NULL;
    }
    
}

bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile) {
    if (doc == NULL || gpxSchemaFile == NULL) {
        return false;
    }
    int validFlag;
    xmlDoc* docXML = gpxDocToXmlDoc(doc);
    validFlag = validateXmlTree((xmlDocPtr) docXML, gpxSchemaFile);
    
    if (validFlag == 1) {
        return true;
    } else {
        return false;
    }
}

bool writeGPXdoc(GPXdoc* doc, char* fileName) {
    if (doc == NULL || fileName == NULL) {
        return false;
    }
    xmlDoc* docXML = gpxDocToXmlDoc(doc);
    /* 
     * Dumping document to stdio or file
     */
    if (xmlSaveFormatFileEnc(fileName, docXML, "UTF-8", 1) == -1) {
        xmlFreeDoc(docXML);
        return false;
    } else {
        xmlFreeDoc(docXML);
        return true;
    }

}

//-----------------------------------------A2 Module 2 Functions---------------------------------------------

float round10(float len) {
    int x;
    if (len >= 0) {
        x = (len+5)/10;
    } else {
        x = (len-5)/10;
    }
        x *= 10;
    return (float)x;
}

float haversine(double lat1, double lon1, double lat2, double lon2) {
    int meters = 6371000;
    float num = sin(((lat2-lat1) * M_PI/180)/2) * sin(((lat2-lat1) * M_PI/180)/2) + cos(lat1 * M_PI/180) * cos(lat2 * M_PI/180) * sin(((lon2-lon1) * M_PI/180)/2) * sin(((lon2-lon1) * M_PI/180)/2);
    return (meters * 2 * (atan2(sqrt(num), sqrt(1-num))));
}

float getRouteLen(const Route *rt) {
    if (rt == NULL) {
        return 0;
    }
    int i;
    float dist = 0;
    
    Node* cur_node = rt->waypoints->head->next;
    for (i = 1; i < rt->waypoints->length; i++) {
        Waypoint* cur = cur_node->data;
        Waypoint* prev = cur_node->previous->data;
        dist += haversine(prev->latitude, prev->longitude, cur->latitude, cur->longitude);
        cur_node = cur_node->next;
    }
    return dist;
}

float getTrackLen(const Track *tr) {
    if (tr == NULL) {
        return 0;
    }
    int i, j;
    float dist = 0;
    int flag = 0;

    Waypoint* cur = NULL;
    Waypoint* prev = NULL;
    
    Node* cur_node = tr->segments->head;
    for (i = 0; i < tr->segments->length; i++) {
        //add distances between segments
        if (flag == 1) {
            TrackSegment* s = cur_node->data;
            Node* n = s->waypoints->head;
            Waypoint* w = n->data;
            dist += haversine(cur->latitude, cur->longitude, w->latitude, w->longitude);
        }
        flag = 1;

        //add distances between waypoints
        TrackSegment* seg = cur_node->data;
        Node* wptNode = seg->waypoints->head->next;
        for (j = 1; j < seg->waypoints->length; j++) {
            cur = wptNode->data;
            prev = wptNode->previous->data;
            dist += haversine(prev->latitude, prev->longitude, cur->latitude, cur->longitude);
            wptNode = wptNode->next;
        }
        cur_node = cur_node->next;
    }
    return dist;
}

int numRoutesWithLength(const GPXdoc* doc, float len, float delta) {
    if (doc == NULL || len < 0 || delta < 0) {
        return 0;
    }
    int i, dist;
    int count = 0;
    Node* cur_node = doc->routes->head;
    for (i = 0; i < doc->routes->length; i++) {
        dist = getRouteLen((Route*)cur_node->data);
        if (dist >= (len - delta) && dist <= (len + delta)) {
            count++;
        }
        cur_node = cur_node->next;
    }
    return count;
}

int numTracksWithLength(const GPXdoc* doc, float len, float delta) {
    if (doc == NULL || len < 0 || delta < 0) {
        return 0;
    }
    int i, dist;
    int count = 0;
    Node* cur_node = doc->tracks->head;
    for (i = 0; i < doc->tracks->length; i++) {
        dist = getTrackLen((Track*)cur_node->data);
        if (dist >= (len - delta) && dist <= (len + delta)) {
            count++;
        }
        cur_node = cur_node->next;
    }
    return count;
}

bool isLoopRoute(const Route* route, float delta) {
    if (route == NULL || delta < 0) {
        return false;
    }
    if (route->waypoints->length >= 4) {
        Node* head = route->waypoints->head;
        Node* tail = route->waypoints->tail;
        Waypoint* wpt1 = head->data;
        Waypoint* wpt2 = tail->data;
        float dist = haversine(wpt1->latitude, wpt1->longitude, wpt2->latitude, wpt2->longitude);
        if (dist <= delta) {
            return true;
        }
    }
    return false;
}

bool isLoopTrack(const Track *tr, float delta) {
    if (tr == NULL || delta < 0) {
        return false;
    }
    int i;
    int num = 0;
    Node* cur_node = tr->segments->head;
    TrackSegment* ts = cur_node->data;
    Waypoint* wpt1 = ts->waypoints->head->data;
    Waypoint* wpt2;
    for (i = 0; i < tr->segments->length; i++) {
        TrackSegment* ts1 = cur_node->data;
        num += ts1->waypoints->length;
        wpt2 = ts1->waypoints->tail->data;
        cur_node = cur_node->next;
    }
    float dist = haversine(wpt1->latitude, wpt1->longitude, wpt2->latitude, wpt2->longitude);
    if (dist <= delta && num >= 4) {
        return true;
    }
    return false;
}

void uselessFunction(void* yes) {
    if (2 == 1+1) {
        return;
    } else {
        printf("HOW\n");
        return;
    }
}

List* getRoutesBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
    if (doc == NULL || delta < 0) {
        return NULL;
    }
    void (*delPtr)(void*) = &uselessFunction;
    char* (*strPtr)(void*) = &routeToString;
    int (*cmpPtr)(const void*, const void*) = &compareRoutes;
    List* list = initializeList(strPtr, delPtr, cmpPtr);

    int i;
    Node* cur_node = doc->routes->head;
    for (i = 0; i < doc->routes->length; i++) {
        Route* r = cur_node->data;
        Waypoint* start = r->waypoints->head->data;
        Waypoint* end = r->waypoints->tail->data;
        if (haversine(start->latitude, start->longitude, sourceLat, sourceLong) <= delta && haversine(end->latitude, end->longitude, destLat, destLong) <= delta) {
            insertBack(list, r);
        }
    }
    if (list->length > 0) {
        return list;
    } else {
        free(list);
        return NULL;
    }
}

List* getTracksBetween(const GPXdoc* doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta) {
    if (doc == NULL || delta < 0) {
        return NULL;
    }
    void (*delPtr)(void*) = &uselessFunction;
    char* (*strPtr)(void*) = &trackToString;
    int (*cmpPtr)(const void*, const void*) = &compareTracks;
    List* list = initializeList(strPtr, delPtr, cmpPtr);

    int i;
    Node* cur_node = doc->tracks->head;
    for (i = 0; i < doc->tracks->length; i++) {
        Track* t = cur_node->data;
        TrackSegment* tsStart = t->segments->head->data;
        TrackSegment* tsEnd = t->segments->tail->data;
        Waypoint* start = tsStart->waypoints->head->data;
        Waypoint* end = tsEnd->waypoints->tail->data;
        if (haversine(start->latitude, start->longitude, sourceLat, sourceLong) <= delta && haversine(end->latitude, end->longitude, destLat, destLong) <= delta) {
            insertBack(list, t);
        }
    }
    if (list->length > 0) {
        return list;
    } else {
        free(list);
        return NULL;
    }
}

//-----------------------------------------A2 Module 3 Functions---------------------------------------------

char* routeToJSON(const Route *rt) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (rt == NULL) {
        strcpy(string, "{}");
        return string;
    }
    strcpy(string, "{\"name\":\"");
    if (rt->name != NULL) {
        strcat(string, rt->name);
    } else {
         strcat(string, "None");
    }
    strcat(string, "\",\"numPoints\":");
    char* s = (char*)malloc(sizeof(char)+1000);
    sprintf(s, "%d", rt->waypoints->length);
    strcat(string, s);
    strcat(string, ",\"len\":");
    sprintf(s, "%.1lf", round10(getRouteLen(rt)));
    strcat(string, s);
    strcat(string, ",\"loop\":");
    if (isLoopRoute(rt, 10)) {
        strcat(string, "true");
    } else {
        strcat(string, "false");
    }
    strcat(string, "}");
    free(s);
    return string;
}

char* trackToJSON(const Track *tr) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (tr == NULL) {
        strcpy(string, "{}");
        return string;
    }
    strcpy(string, "{\"name\":\"");
    if (tr->name != NULL) {
        strcat(string, tr->name);
    } else {
         strcat(string, "None");
    }
    strcat(string, "\",\"numPoints\":");
    char* s = (char*)malloc(sizeof(char)+1000);
    int numPoints = 0;
    Node* cur_node = tr->segments->head;
    int i;
    for (i = 0; i < tr->segments->length; i++) {
        TrackSegment* ts = cur_node->data;
        numPoints += ts->waypoints->length;
        cur_node = cur_node->next;
    }
    sprintf(s, "%d", numPoints);
    strcat(string, s);
    strcat(string, ",\"len\":");
    sprintf(s, "%.1lf", round10(getTrackLen(tr)));
    strcat(string, s);
    strcat(string, ",\"loop\":");
    if (isLoopTrack(tr, 10)) {
        strcat(string, "true");
    } else {
        strcat(string, "false");
    }
    strcat(string, "}");
    free(s);
    return string;
}

char* routeListToJSON(const List *list) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (list == NULL) {
        strcpy(string, "[]");
        return string;
    }
    int i;
    int flag = 0;
    strcpy(string, "[");
    Node* cur_node = list->head;
    for (i = 0; i < list->length; i++) {
        if (flag == 1) {
            strcat(string, ",");
        }
        flag = 1;
        char* s = routeToJSON((Route*)cur_node->data);
        strcat(string, s);
        free(s);
        cur_node = cur_node->next;
    }
    strcat(string, "]");
    return string;
}

char* trackListToJSON(const List *list) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (list == NULL) {
        strcpy(string, "[]");
        return string;
    }
    int i;
    int flag = 0;
    strcpy(string, "[");
    Node* cur_node = list->head;
    for (i = 0; i < list->length; i++) {
        if (flag == 1) {
            strcat(string, ",");
        }
        flag = 1;
        char* s = trackToJSON((Track*)cur_node->data);
        strcat(string, s);
        free(s);
        cur_node = cur_node->next;
    }
    strcat(string, "]");
    return string;
}

char* GPXtoJSON(const GPXdoc* gpx) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (gpx == NULL) {
        strcpy(string, "{}");
        return string;
    }
    strcpy(string, "{\"version\":"); 
    char* s = (char*)malloc(sizeof(char)+1000);
    sprintf(s, "%.1lf", gpx->version);
    strcat(string, s);
    strcat(string, ",\"creator\":\"");
    strcat(string, gpx->creator);

    strcat(string, "\",\"numWaypoints\":");
    sprintf(s, "%d", getNumWaypoints(gpx));
    strcat(string, s);

    strcat(string, ",\"numRoutes\":");
    sprintf(s, "%d", getNumRoutes(gpx));
    strcat(string, s);

    strcat(string, ",\"numTracks\":");
    sprintf(s, "%d", getNumTracks(gpx));
    strcat(string, s);
    
    strcat(string, "}");
    free(s);
    
    return string;
}

//-----------------------------------------A2 Module 3 Bonus Functions---------------------------------------------

void addWaypoint(Route *rt, Waypoint *pt) {
    if (rt == NULL || pt == NULL) {
        return;
    }
    insertBack(rt->waypoints, pt);
}

void addRoute(GPXdoc* doc, Route* rt) {
    if (doc == NULL || rt == NULL) {
        return;
    }
    insertBack(doc->routes, rt);
}

GPXdoc* JSONtoGPX(const char* gpxString) {
    if (gpxString == NULL) {
        return NULL;
    }

    double ver;
    char* creator = malloc(sizeof(char) * 30);
    sscanf(gpxString, "{\"version\":%lf,\"creator\":\"%[^\n]\"}", &ver, creator);
    strtok(creator, "\"}");

    GPXdoc* doc = (GPXdoc*)malloc(sizeof(GPXdoc));
    doc->version = ver;
    doc->creator = creator;
    strcpy(doc->namespace, "http://www.topografix.com/GPX/1/1");

    //initialize the lists
    void (*delPtr)(void*) = &deleteWaypoint;
    char* (*strPtr)(void*) = &waypointToString;
    int (*cmpPtr)(const void*, const void*) = &compareWaypoints;

    void (*delPtr2)(void*) = &deleteRoute;
    char* (*strPtr2)(void*) = &routeToString;
    int (*cmpPtr2)(const void*, const void*) = &compareRoutes;

    void (*delPtr3)(void*) = &deleteTrack;
    char* (*strPtr3)(void*) = &trackToString;
    int (*cmpPtr3)(const void*, const void*) = &compareTracks;

    List* waypointList = initializeList(strPtr, delPtr, cmpPtr);
    List* routeList = initializeList(strPtr2, delPtr2, cmpPtr2);
    List* trackList = initializeList(strPtr3, delPtr3, cmpPtr3);

    doc->waypoints = waypointList;
    doc->routes = routeList;
    doc->tracks = trackList;

    return doc;
}

Waypoint* JSONtoWaypoint(const char* gpxString) {
    if (gpxString == NULL) {
        return NULL;
    }
    double lat, lon;
    sscanf(gpxString, "{\"lat\":%lf,\"lon\":%lf}", &lat, &lon);

    Waypoint* wpt = (Waypoint*)malloc(sizeof(char));
    wpt->name = malloc(sizeof(char) * 1000);
    strcpy(wpt->name, "");
    wpt->latitude = lat;
    wpt->longitude = lon;

    void (*delPtr)(void*) = &deleteGpxData;
    char* (*strPtr)(void*) = &gpxDataToString;
    int (*cmpPtr)(const void*, const void*) = &compareGpxData;
    wpt->otherData = initializeList(strPtr, delPtr, cmpPtr);
    
    return wpt;
}

Route* JSONtoRoute(const char* gpxString) {
    if (gpxString == NULL) {
        return NULL;
    }
    char* name = malloc(sizeof(char) * 100);
    strcpy(name, gpxString);

    Route* rt = (Route*)malloc(sizeof(Route));

    rt->name = name;

    void (*delPtr)(void*) = &deleteWaypoint;
    char* (*strPtr)(void*) = &waypointToString;
    int (*cmpPtr)(const void*, const void*) = &compareWaypoints;
    rt->waypoints = initializeList(strPtr, delPtr, cmpPtr);

    void (*delPtr2)(void*) = &deleteGpxData;
    char* (*strPtr2)(void*) = &gpxDataToString;
    int (*cmpPtr2)(const void*, const void*) = &compareGpxData;
    rt->otherData = initializeList(strPtr2, delPtr2, cmpPtr2);

    return rt;
}

//-----------------------------------------A3 Functions---------------------------------------------

char* getGPXJSON(char* filename) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* gpxJSON;
    if (doc != NULL) {
        gpxJSON = GPXtoJSON(doc);
    } else {
        return "error";
    }
    deleteGPXdoc(doc);
    return gpxJSON;
}

char* getRoutesJSON(char* filename) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* routesJSON;
    if (doc != NULL) {
        routesJSON = routeListToJSON(doc->routes);
    } else {
        return "error";
    }
    deleteGPXdoc(doc);
    return routesJSON;
}

char* getTracksJSON(char* filename) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* tracksJSON;
    if (doc != NULL) {
        tracksJSON = trackListToJSON(doc->tracks);
    } else {
        return "error";
    }
    deleteGPXdoc(doc);
    return tracksJSON;
}

char* gpxDataToJSON(GPXData* data) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (data == NULL) {
        strcpy(string, "{}");
        return string;
    }
    strcpy(string, "{\"name\":\"");
    strcat(string, data->name);
    strcat(string, "\",\"value\":\"");
    char* s = malloc(sizeof(char)*500);
    strcpy(s, data->value);
    strtok(s, "\n");
    strcat(string, s);
    free(s);
    strcat(string, "\"}");
    return string;
}

char* gpxDataListToJSON(char* filename, char* name) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* string = (char*)malloc(sizeof(char)*5000);

    char* RoT = (char*)malloc(sizeof(char)*10);
    int num;

    sscanf(name, "%s %d", RoT, &num);

    if (strcmp(RoT, "Route") == 0) {
        int i;
        int flag = 0;
        strcpy(string, "[");
        Node* cur_node = doc->routes->head;
        
        for (i = 1; i < num; i++) {
            cur_node = cur_node->next;
        }

        Route* rt = cur_node->data;
        Node* cur_node2 = rt->otherData->head;
        int j;
        for (j = 0; j < rt->otherData->length; j++) {
            if (flag == 1) {
                strcat(string, ",");
            }
            flag = 1;

            GPXData* data = cur_node2->data;
            strcat(string, gpxDataToJSON(data));
            cur_node2 = cur_node2->next;
        }
        
        strcat(string, "]");
    } else if (strcmp(RoT, "Track") == 0) {
        int i;
        int flag = 0;
        strcpy(string, "[");
        Node* cur_node = doc->tracks->head;
        
        for (i = 1; i < num; i++) {
            cur_node = cur_node->next;
        }

        Track* tr = cur_node->data;
        Node* cur_node2 = tr->otherData->head;
        int j;
        for (j = 0; j < tr->otherData->length; j++) {
            if (flag == 1) {
                strcat(string, ",");
            }
            flag = 1;

            GPXData* data = cur_node2->data;
            strcat(string, gpxDataToJSON(data));

            cur_node2 = cur_node2->next;
        }
        //get waypoint gpxData
        Node* cur_node3 = tr->segments->head;
        for (j = 0; j < tr->segments->length; j++) {
            TrackSegment* ts = cur_node3->data;
            Node* cur_node4 = ts->waypoints->head;
            int p;
            for (p = 0; p < ts->waypoints->length; p++) {
                Waypoint* wpt = cur_node4->data;
                Node* cur_node5 = wpt->otherData->head;
                int q;
                for (q = 0; q < wpt->otherData->length; q++) {
                    if (flag == 1) {
                        strcat(string, ",");
                    }
                    flag = 1;
                    GPXData* data = cur_node5->data;
                    strcat(string, gpxDataToJSON(data));
                    cur_node5 = cur_node5->next;
                }
                cur_node4 = cur_node4->next;
            }
            cur_node3 = cur_node3->next;
        }
        
        strcat(string, "]");
    } else {
        strcpy(string, "[]");
    }
    return string;

}

void renameComp(char* filename, char* newName, char* comp) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* RoT = (char*)malloc(sizeof(char)*10);
    int num;

    sscanf(comp, "%s %d", RoT, &num);

    if (strcmp(RoT, "Route") == 0) {
        Node* cur_node = doc->routes->head;
        
        int i;
        for (i = 1; i < num; i++) {
            cur_node = cur_node->next;
        }
        Route* rt = cur_node->data;
        strcpy(rt->name, newName);
    } else if (strcmp(RoT, "Track") == 0) {
        Node* cur_node = doc->tracks->head;
        
        int i;
        for (i = 1; i < num; i++) {
            cur_node = cur_node->next;
        }
        Track* tr = cur_node->data;
        strcpy(tr->name, newName);
    }
    bool val = validateGPXDoc(doc,  "./parser/gpx.xsd");
    if (val) {
        writeGPXdoc(doc, filename);
    } else {
        printf("Fail\n");
    }
}

bool createNewGPX(char* filename, char* JSONDoc) {
    GPXdoc* doc = JSONtoGPX(JSONDoc);
    bool flag = false;
    if (validateGPXDoc(doc, "./parser/gpx.xsd")) {
        flag = writeGPXdoc(doc, filename);
    }
    return flag;
}

bool addRouteToFile(char* filename, char* routeJSON, char* waypointsJSON, int num) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* waypointList[100];
    int i;
    for (i = 0; i < 100; i++) {
        waypointList[i] = malloc(sizeof(char) * 300);
    }
    char newJSON[1000];
    for (i = 0; i < strlen(waypointsJSON); i++) {
        newJSON[i] = waypointsJSON[i];
    }
    char* token = malloc(sizeof(char) * 300);

    const char del[2] = "|";

    token = strtok(newJSON, del);
    int j = 0;
    while(token != NULL) {
      waypointList[j] = token;
      j++;
      token = strtok(NULL, del);
    }

    printf("\tPlease: %d\n", num);
    
    Route* rt = JSONtoRoute(routeJSON);
    for (i = 0; i < num; i++) {
        printf("\tTesting: %s\n", waypointList[i]);
        Waypoint* wpt = JSONtoWaypoint(waypointList[i]);
        addWaypoint(rt, wpt);
    }
    addRoute(doc, rt);
    bool flag = false;
    remove(filename);
    if (validateGPXDoc(doc, "./parser/gpx.xsd")) {
        flag = writeGPXdoc(doc, filename);
    }
    return flag;
}

char* pointToJSON(Waypoint* wpt, int index) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (wpt == NULL) {
        strcpy(string, "{}");
        return string;
    }
    strcpy(string, "{\"name\":\"");
    if (wpt->name != NULL) {
        strcat(string, wpt->name);
    } else {
        strcat(string, "NULL");
    }
    strcat(string, "\",\"lat\":");
    char* s = (char*)malloc(sizeof(char)*1000);
    sprintf(s, "%lf", wpt->latitude);
    strcat(string, s);
    strcat(string, ",\"lon\":");
    sprintf(s, "%lf", wpt->longitude);
    strcat(string, s);
    strcat(string, ",\"index\":");
    sprintf(s, "%d", index);
    strcat(string, s);
    strcat(string, "}");
    free(s);
    return string;
}

char* routeListToPointJSON(const List *list) {
    char* string = (char*)malloc(sizeof(char)*5000);
    if (list == NULL) {
        strcpy(string, "[]");
        return string;
    }
    int i, j;
    int flag = 0;
    strcpy(string, "[");
    Node* cur_node = list->head;
    for (i = 0; i < list->length; i++) {
        Route* rt = (Route*)cur_node->data;
        Node* cur_node2 = rt->waypoints->head;
        for (j = 0; j < list->length; j++) {
            if (flag == 1) {
                strcat(string, ",");
            }
            flag = 1;
            Waypoint* wpt = cur_node2->data;
            char* s = pointToJSON(wpt, j);
            strcat(string, s);
            free(s);
            cur_node2 = cur_node2->next;
        }
        cur_node = cur_node->next;
    }
    strcat(string, "]");
    return string;
}

char* getPointsJSON(char* filename) {
    GPXdoc* doc = createValidGPXdoc(filename, "./parser/gpx.xsd");
    char* pointsJSON;
    if (doc != NULL) {
        pointsJSON = routeListToPointJSON(doc->routes);
    } else {
        return "error";
    }
    deleteGPXdoc(doc);
    return pointsJSON;
}
