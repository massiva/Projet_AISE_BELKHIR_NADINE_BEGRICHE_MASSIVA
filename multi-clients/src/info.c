
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fnmatch.h>
#include <sys/types.h>
#include <string.h>
#include <sys/sysctl.h>



#define BUFSZ 1024
#define MEM_FIELD_NAME "VmSize:"
#include <time.h>
clock_t start, end;
double cpu_time_used;


void getMemoryInfo()
{
    FILE *meminfo = fopen("/proc/meminfo", "r");
    int totalMemory = 0;
    if(meminfo == NULL)
    {
        printf("erreur repertoire vide!!");
        exit(-1);
    }
    char buff[256];
    while(fgets(buff, sizeof(buff), meminfo))
    {
        int ramKB;
        if(sscanf(buff, "MemTotal: %d kB", &ramKB) == 1)
        {
            totalMemory = ramKB;

        }
    }
   (fclose(meminfo));
   /* if(fclose(meminfo) != 0)
    {
           (fclose(meminfo);
        exit(-1);
    }*/
    printf("\tMemory capacity : %d\n \n", totalMemory);
}

void processdir(const struct dirent *piddir)
{
    char path[BUFSZ];
    char line[BUFSZ];
    char *memstr;
    FILE *pidmemfile;
    int  offset = strlen(MEM_FIELD_NAME);
    int time = 0;
    
    /* Construct full path of the file containt memory-info if this PID */
    snprintf(path, BUFSZ, "/proc/%s/status", piddir->d_name);
    
    /* Open the file */
    pidmemfile = fopen(path, "rt");

    /* Read line-by-line until we found the line we want */
    while (fgets(line, BUFSZ, pidmemfile) != NULL) {
        memstr = strstr(line, MEM_FIELD_NAME);
        if (memstr != NULL) {  /* Found our line */
            start = clock();
            memstr += offset;
            end = clock();

           ++memstr;//aligned code
            
            
           cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
           printf("\t%s \t %lf \t %s \n ", piddir->d_name,cpu_time_used ,memstr);

           //printf("Temps d'execution=%lf \r", cpu_time_used);
          
            break;
        }
}
    
    fclose(pidmemfile);
}



