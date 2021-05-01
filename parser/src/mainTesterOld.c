#include "GPXParser.c"

void A2test() {
    //GPXdoc* d = createGPXdoc("simple.gpx");
    //xmlDoc* docXML = gpxDocToXmlDoc(d);
    //printf("PLZ!!!: [%d]\n", validateXmlTree(xmlReadFile("simple.gpx", NULL, 0), "gpx.xsd"));
    printf("Round 10: [%f]\n", round10(527));

    bool test;
    GPXdoc* d = createValidGPXdoc("simple.gpx", "gpx.xsd");
    if (d == NULL) {
        printf("YOU IDIOT\n");
    }
    test = writeGPXdoc(d, "testFile.xml");
    if (test == true) {
        printf("success\n");
    } else {
        printf("fail\n");
    }   
    test = validateGPXDoc(d, "gpx.xsd");
    if (test == true) {
        printf("success\n");
    } else {
        printf("fail\n");
    }
    printf("Length: [%f]\n", getRouteLen(getRoute(d, "Some route")));
    printf("Length: [%f]\n", getTrackLen(getTrack(d, "Mount Steele Trail")));
    printf("Num: [%d]\n", numRoutesWithLength(d, 100, 50));
    printf("Num: [%d]\n", numTracksWithLength(d, 332, 6));
    test = isLoopRoute(getRoute(d, "Some route"), 100);
    if (test == true) {
        printf("success\n");
    } else {
        printf("fail\n");
    }
    test = isLoopTrack(getTrack(d, "Mount Steele Trail"), 300);
    if (test == true) {
        printf("success\n");
    } else {
        printf("fail\n");
    }
    char* s = GPXdocToString(d);
    printf(s);
    free(s);
    s = routeToJSON(getRoute(d, "Some route"));
    printf("%s\n", s);
    free(s);
    s = trackToJSON(getTrack(d, "Mount Steele Trail"));
    printf("%s\n", s);
    free(s);
    s =routeListToJSON(d->routes);
    printf("%s\n", s);
    free(s);
    s = trackListToJSON(d->tracks);
    printf("%s\n", s);
    free(s);
    s = GPXtoJSON(d);
    printf("%s\n", s);
    free(s);
    deleteGPXdoc(d);
}

void A1test() {
    //printf("hello!\n");
    GPXdoc* d = createGPXdoc("simple.gpx");
    char* s = GPXdocToString(d);
    printf(s);
    free(s);

    printf("Num Waypoints %d\n", getNumWaypoints(d));
    printf("Num Routes %d\n", getNumRoutes(d));
    printf("Num Tracks %d\n", getNumTracks(d));
    printf("Num Segments %d\n", getNumSegments(d));
    printf("Num GPXData %d\n", getNumGPXData(d));

    s = waypointToString(getWaypoint(d, "Steele Cabin"));
    printf("\nFind Waypoint: [%s]\n", s);
    free(s);
    s = trackToString(getTrack(d, "Mount Steele Trail"));
    printf("\nFind Track: [%s]\n", s);
    free(s);
    s = routeToString(getRoute(d, "Some route"));
    printf("\nFind Route: [%s]\n", s);
    free(s);

    deleteGPXdoc(d);
}

int main ( int argc, char *argv[] ) {
    A2test();
}
