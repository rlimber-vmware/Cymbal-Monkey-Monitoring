#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for sleep function

void get_cpu_usage(FILE *output_file) {
    FILE *fp;
    char buffer[1024];
    
    // Open the /proc/stat file for reading
    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        perror("Error opening /proc/stat");
        exit(1);
    }

    // Read the first line which contains overall CPU statistics
    fgets(buffer, sizeof(buffer), fp);

    // Parse the CPU metrics from the buffer
    unsigned long long int user, nice, system, idle;
    sscanf(buffer, "cpu %llu %llu %llu %llu", &user, &nice, &system, &idle);

    // Calculate CPU usage
    unsigned long long int total = user + nice + system + idle;
    unsigned long long int usage = user + nice + system;
    double percentage = (double)usage / total * 100;

    // Print the CPU usage percentage
    fprintf(output_file,"CPU Usage: %.2f%%\n", percentage);

    // Close the file
    fclose(fp);
}

void get_hostname(FILE *output_file){

    FILE *fp;
    char buffer[1024];
    
    fp = fopen("/etc/hostname", "r");
    if (fp == NULL) {
        perror("Error opening /etc/hostname");
    }
    fgets(buffer, sizeof(buffer), fp);
    fprintf(output_file,"%s", buffer);

    fclose(fp);

}

void get_memory_usage(FILE *output_file) {
    FILE *fp;
    char buffer[1024];

    // Open the /proc/meminfo file for reading
    fp = fopen("/proc/meminfo", "r");
    if (fp == NULL) {
        perror("Error opening /proc/meminfo");
        exit(1);
    }

    // Read memory usage information
    unsigned long long int total, free, buffers, cached;
    while (fgets(buffer, sizeof(buffer), fp)) {
        if (sscanf(buffer, "MemTotal: %llu kB", &total) == 1) {}
        else if (sscanf(buffer, "MemFree: %llu kB", &free) == 1) {}
        else if (sscanf(buffer, "Buffers: %llu kB", &buffers) == 1) {}
        else if (sscanf(buffer, "Cached: %llu kB", &cached) == 1) {}
    }

    // Calculate used memory
    unsigned long long int used = total - free - buffers - cached;
    double percentage = (double)used / total * 100;
    // Print memory usage information
    fprintf(output_file,"Memory Usage: %.2f%%\n", percentage);
    // Close the file
    fclose(fp);
}

void create_output_file(){
    FILE *output_file;
    FILE *fp;
    char buffer[1024];

    fp = fopen("/etc/hostname", "r");
    if (fp == NULL) {
        perror("Error opening /etc/hostname");
    }
    fgets(buffer, sizeof(buffer), fp);
    
    // Remove newline character if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    char filename[256] = "";
    strcat(filename, "/root/");
    strcat(filename, buffer);
    strcat(filename, ".txt");

    output_file = fopen(filename, "w"); //path of the output file
    if (output_file == NULL) {
        perror("Error opening output file");
        exit(1);
    }

    // Call functions to retrieve and store usage information
    get_hostname(output_file);
    get_cpu_usage(output_file);
    get_memory_usage(output_file);


    // Close the output file
    fclose(output_file);

}

int main() {
    
    while(1){
    create_output_file();
    sleep(5);
    }
    return 0;
}
