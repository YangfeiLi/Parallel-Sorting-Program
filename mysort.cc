#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>
using namespace std;

void print_guideline();
void segment_vector(int n, vector<long long > nums, vector< vector< long long > > &nums_seg);
void swap(long long *a, long long *b);
void bubble_sort(vector<long long> &nums);
void print_vector(vector<long long> &nums);
void sort_by_process(int n, vector< vector < long long > > &nums_seg);
void pipe_config(int& pid, int& pipe_id, int n, int down[n][2], int up[n][2]);
void merge_segments(vector< vector< long long > > &nums_seg);
vector<long long> merge(vector<long long> left, vector<long long> right);
void* bubble_sort_by_threads(void* segment);
void sort_by_threads(int n, vector<vector<long long> > &nums_seg);


int main(int argc, char *argv[])
{
    print_guideline();  // print the usage guideline

    if (argc <= 1) {    // invalid input
        fprintf(stderr, "Fullname: Yangfei Li\nSEAS login: lyf\n");
        printf("Invalid input!\n");
        exit(1);
    }

    int n = 4;
    int thread_flag = 0;
    int lexicographic_flag = 0;

    int c;
    while ((c = getopt (argc, argv, "n:t")) != -1) {   // parse input
        switch (c)
        {
            case 'n':       // number of processes/threads
                n = atoi(optarg);
                if (n <= 0) {
                    fprintf(stderr, "Invalid number of processes.\n");
                    printf("Please enter a positive integer number of processes!\n");
                    exit(1);
                }
                break;
            case 't':       // sort by thread
                thread_flag = 1;
                break;
            default:        // sort by processes
                fprintf(stderr, "Undefined option!\n");
                printf("Undefined option! Please enter according to the guideline!\n");
                exit(1);
        }
    }

    vector <long long> nums;    // store the numbers in a vector
    for (int i = optind; i < argc; i++) {       // parse the file
        ifstream inFile(argv[i]);

        long long cur_num = 0;
        if (inFile.is_open()) {
            while (inFile >> cur_num) {
                nums.push_back(cur_num);
            }
            inFile.close();
        } else {
            fprintf(stderr, "Open file error!\n");
            printf("Can't open the file: %s\n", argv[i]);
            exit(1);
        }
    }

    switch (n)
    {
        case 1:     // sort directly
            printf("n = 1, sort directly!\n");
            bubble_sort(nums);
            print_vector(nums);
            return 0;
        default:    // sort by subprocess or threads
            printf("n > 1, sort by subprocess or threads!\n");
            vector< vector < long long > > nums_seg;
            segment_vector(n, nums, nums_seg);

            switch (thread_flag)
            {
                case 1:     // sort by threads
                    printf("sort by thread\n");
                    sort_by_threads(n, nums_seg);
                    break;
                default:    // sort by processes
                    printf("sort by process\n");
                    sort_by_process(n, nums_seg);
            }

            merge_segments(nums_seg);   // merge the sorted vectors
            nums = nums_seg[0];         // take out sorted result from the number segments
            print_vector(nums);         // print the result

            // clear memory
            for (vector<long long> seg : nums_seg) {
                seg.clear();
            }
            nums_seg.clear();
    }

    nums.clear();
    return 0;
}

/*
 * print the usage guideline of the program
 */
void print_guideline()
{
    cout << "==================================================================================" << "\n";
    cout << "Input Guideline: ./mysort [-n num_of_process] [-t] [-L] [target files...]\n";
    cout << "==================================================================================" << "\n";
}

/*
 * segment the long vecter into segments for each thread/process
 */
void segment_vector(int n, vector<long long > nums, vector< vector< long long > > &nums_seg)
{
    auto size = nums.size() / n;
    auto counter = 0;
    while (counter < nums.size()) {
        vector<long long> seg;
        for (auto i = counter; i < min(counter + size, nums.size()); ++i) {
            seg.push_back(nums[i]);
        }
        nums_seg.push_back(seg);
        seg.clear();
        counter += size;
    }
}

/*
 * swap operation in sort
 */
void swap(long long *a, long long *b)
{
    long long temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * bubble sort by process
 */
void bubble_sort(vector<long long> &nums)
{
    for (auto i = 0; i < nums.size() - 1; i ++) {
        for (auto j = 0; j < nums.size() - i - 1; j ++) {
            if (nums[j] > nums[j + 1]) {
                swap(&nums[j], &nums[j + 1]);
            }
        }
    }
}

/*
 * print the vector
 */
void print_vector(vector<long long> &nums)
{
    for (long long i = 0; i < nums.size(); ++i) {
        printf("%lld\t%lld\n", i + 1, nums[i]);
    }
}

/*
 * sort by process: create up and down pipes for each process
 * - In down pipe:
 *      - parent process write original data
 *      - child read the data
 * - In up pipe:
 *      - child process write the sorted data
 *      - parent read the sorted data and wait for all the children to finish
 */
void sort_by_process(int n, vector< vector< long long > > &nums_seg)
{
    int down[n][2];
    int up[n][2];
    int pid;
    int pipe_id;

    // initialize the pipes and close some ends
    pipe_config(pid, pipe_id, n, down, up);

    // IPC in the down pipes;
    vector<long long> seg; // store the sorted numbers
    if (pid > 0) {
        /* parent  process */
        for (int i = 0; i < n; ++i) {
            // transfer the original data to child process
            FILE* parent_write_file = fdopen(down[i][1], "w");
            if (parent_write_file == NULL) {
                fprintf(stderr, "Parent write file fail!\n");
                printf("Parent write file fail!\n");
                exit(1);
            }

            for (long long num : nums_seg[i]) {
                fprintf(parent_write_file, "%lld\n", num);
            }

            fclose(parent_write_file);
            close(down[i][1]);
        }
        // parent write successfully
    }
    else if (pid == 0)
    {
        /* child process */
        // child read from the down pipe and sort the array
        FILE* child_read_file = fdopen(down[pipe_id][0], "r");
        if (child_read_file == NULL) {
            fprintf(stderr, "Child read file fail!\n");
            printf("Child read file fail!\n");
            exit(1);
        }

        char buf[100];
        for(auto i = 0; i < nums_seg[pipe_id].size(); i++) {
            if (fgets(buf, sizeof(buf), child_read_file) != NULL) {
                long long val = stoll(buf);
                seg.push_back(val);
            }
        }

        fclose(child_read_file);
        close(down[pipe_id][0]);
        // child read successfully
    }
    else
    {
        fprintf(stderr, "Fork error!");
        printf("Fork error after pipe config!");
        exit(1);
    }

    // IPC in the up pipes;
    if (pid > 0) {
        /* parent  process */
        // parent process read the sorted data
        for (auto i = 0; i < n; ++i) {
            FILE* parent_read_file = fdopen(up[i][0], "r");
            if (parent_read_file == NULL) {
                fprintf(stderr, "Parent read file fail!\n");
                printf("Parent read file fail!\n");
                exit(1);
            }

            char buf[100];
            for (long long &num : nums_seg[i]) {
                if (fgets(buf, sizeof(buf), parent_read_file) != NULL) {
                    long long val = stoll(buf);
                    num = val;
                }
            }

            fclose(parent_read_file);
            close(up[i][0]);

            // wait all the children to finish
            for (int i = 0; i < n; i++) {
                wait(NULL);
            }
        }
    }
    else if (pid == 0)
    {
        /* child process */
        // child transfer the sorted array to parent
        if (seg.size() > 0) {
            bubble_sort(seg);
            FILE* child_write_file = fdopen(up[pipe_id][1], "w");
            if (child_write_file == NULL) {
                fprintf(stderr, "Child write file fail!\n");
                printf("Child write file fail!\n");
                exit(1);
            }

            for (long long num : seg) {
                fprintf(child_write_file, "%lld\n", num);
            }

            fclose(child_write_file);
            close(up[pipe_id][1]);
            // Child write successfully
            exit(0);
        } else {
            fprintf(stderr, "No data read from pipe! (sort by process)\n");
            printf("No data read from pipe! (sort by process)\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr, "Fork error!");
        printf("Fork error after pipe config!");
        exit(1);
    }

    seg.clear();
}

/*
 * Fork n child processes and initialize the pipes
 * Close the read-end of down pipe and write-end of up pipe for parent process
 * Close the read-end of up pipe and write-end of down pipe for child process
 */
void pipe_config(int& pid, int& pipe_id, int n, int down[n][2], int up[n][2])
{
    for (int i = 0; i < n; ++ i) {
        pipe(up[i]);
        pipe(down[i]);
        pid = fork();

        if (pid > 0) {
            /* parent  process */
            close(down[i][0]);      // read end of down pipe
            close(up[i][1]);        // write end of up pipe
        } else if (pid == 0) {
            /* child process */
            pipe_id = i;
            close(down[i][1]);      // write end of down pipe
            close(up[i][0]);        // read end of up pipe
            break;
        } else {
            fprintf(stderr, "Fork error!");
            printf("Fork error!");
            exit(1);
        }
    }
}

/*
 * merge two sorted vectors
 */
vector<long long> merge(vector<long long> left, vector<long long> right)
{
    vector<long long> merged_vector;
    auto i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
        if (left[i] < right[j]) {
            merged_vector.push_back(left[i]);
            i ++;
        } else {
            merged_vector.push_back(right[j]);
            j ++;
        }
    }
    while (i < left.size()) {
        merged_vector.push_back(left[i]);
        i ++;
    }
    while (j < right.size()) {
        merged_vector.push_back(right[j]);
        j ++;
    }
    return merged_vector;
}

/*
 * merge sorted vectors;
 */
void merge_segments(vector< vector<long long > > &nums_seg)
{
    while(nums_seg.size() > 1) {
        // merge the vectors two by two
        nums_seg.push_back(merge(nums_seg[0], nums_seg[1]));
        // remove the two sorted vectors
        nums_seg.erase(nums_seg.begin());
        nums_seg.erase(nums_seg.begin());
    }   // At last, only the long sorted vector stays in the nums_seg
}

/*
 * sort by threads
 * - create n threads and for each thread, give it a segment for sorting.
 * - after sorting, the thread read the sorted data from the pipe and push it back to the segment
 * - wait for all the threads finish
 */
void sort_by_threads(int n, vector<vector<long long> > &nums_seg)
{
    // create thread and pipe array;
    pthread_t tids[n];
    int up[n][2];
    int down[n][2];
    int pipe_id;

    for (int i = 0; i < n; ++i) {
        // initialize pipes
        pipe_id = i;
        pipe(down[i]);
        pipe(up[i]);
        if (nums_seg[i].size() <= 0) {      // error
            fprintf(stderr, "Empty number segments\n");
            printf("Empty number segments");
            exit(1);
        } else {
            // create a thread and let this thread sort this segment
            int errorno = pthread_create(&tids[i], NULL, bubble_sort_by_threads, &nums_seg[i]);
            if (errorno != 0) {
                fprintf(stderr, "Error when create threads, error number is %d", errorno);
                printf("Error when create threads, error number is %d", errorno);
                exit(1);
            }
        }
    }

    vector <long long> segment;
    if (pthread_self() == 0)
    {
        // read the sorted data
        FILE *child_read_file = fdopen(down[pipe_id][0], "r");
        if (child_read_file == NULL)
        {
            fprintf(stderr, "Child thread read fail!");
            printf("Child thread read fail!");
            exit(1);
        }
        else
        {
            char buf[100];
            for(int i = 0; i < nums_seg[pipe_id].size(); i++)
            {
                // push the sorted data into the segment vector
                fgets(buf, sizeof(buf), child_read_file);
                long long val = stoll(buf);
                segment.push_back(val);
            }
            fclose(child_read_file);
        }
    }

    // thread wait
    void *status;
    for (int i = 0; i < n; ++ i) {
        if (nums_seg[i].size() > 0) {
            pthread_join(tids[i], &status);
        } else {
            fprintf(stderr, "Empty segment");
            printf("Empty segment");
            exit(0);
        }
    }

    // close pipes
    for (int i = 0; i < n; ++i) {
        close(down[i][0]);
        close(down[i][1]);
        close(up[i][0]);
        close(up[i][1]);
    }
}

/*
 * bubble sort method for threads because pthread_create() need a void* start_routine
 */
void* bubble_sort_by_threads(void* segment)
{
    vector< long long >* nums = (vector< long long >*) segment;
    for (auto i = 0; i < (*nums).size() - 1; i++) {
        for (auto j = 0; j < (*nums).size() - i - 1; j++) {
            if ((*nums)[j] > (*nums)[j + 1]) {
                swap((*nums)[j], (*nums)[j + 1]);
            }
        }
    }

    pthread_exit(0);
}

