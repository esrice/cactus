#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#include "cactus.h"

int main(int argc, char *argv[]) {
    assert(argc >= 5);

    if (strcmp(argv[4], "INFO") == 0) {
        st_setLogLevel(ST_LOGGING_INFO);
    }
    else if (strcmp(argv[4], "DEBUG") == 0) {
        st_setLogLevel(ST_LOGGING_DEBUG);
    }

    stKVDatabaseConf *kvDatabaseConf = stKVDatabaseConf_constructFromString(argv[1]);
    CactusDisk *cactusDisk = cactusDisk_construct(kvDatabaseConf, 0);
    st_logInfo("Set up the flower disk\n");

    FILE *fileHandle = fopen(argv[2], "w");

    int32_t includeTerminalFlowers;
    int32_t i = sscanf(argv[3], "%i", &includeTerminalFlowers);
    assert(i == 1);
    for (i = 5; i < argc; i++) {
        Flower *flower = cactusDisk_getFlower(cactusDisk, cactusMisc_stringToName(argv[i]));
        assert(flower != NULL);
        if(!flower_isLeaf(flower)) {
            assert(flower_builtBlocks(flower)); //This recursion depends on the block structure having been properly defined for all nodes.
            st_logInfo("Parsed the flower %s\n", argv[i]);
            Flower_GroupIterator *groupIterator = flower_getGroupIterator(flower);
            Group *group;
            while ((group = flower_getNextGroup(groupIterator)) != NULL) {
                if (!group_isLeaf(group)) {
                    Flower *nestedFlower = group_getNestedFlower(group);
                    assert(nestedFlower != NULL);
                    //assert(flower_builtBlocks(nestedFlower)); //This recursion depends on the block structure having been properly defined for all nodes.
                    if(includeTerminalFlowers || !flower_isTerminal(flower)) {
                        fprintf(fileHandle, "%s %" PRIi64 " \n", cactusMisc_nameToStringStatic(flower_getName(nestedFlower)), flower_getTotalBaseLength(nestedFlower));
                    }
                }
            }
            flower_destructGroupIterator(groupIterator);
        }
    }
    fclose(fileHandle);
    cactusDisk_destruct(cactusDisk);
    stKVDatabaseConf_destruct(kvDatabaseConf);
    st_logInfo("Am finished\n");
    return 0;
}