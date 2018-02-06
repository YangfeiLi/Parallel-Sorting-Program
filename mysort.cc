#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <fstream>

using namespace std;

void segment_vector(int n, vector<long long > nums, vector< vector< long long > > &nums_seg);
void swap(long long *a, long long *b);
void bubble_sort(vector<long long> &nums);
void print_vector(vector<long long> &nums);
void sort_by_process(int n, vector< vector < long long > > &nums_seg);
void pipe_config(int& pid, int& pipe_id, int n, int down[n][2], int up[n][2]);
void merge_segments(vector< vector< long long > > &nums_seg);
vector<long long> merge(vector<long long> left, vector<long long> right);

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        fprintf(stderr, "Fullname: Yangfei Li\nSEAS login: lyf\n");
        printf("Invalid input!\n");
        exit(1);
    }

    int n = 4;
    int thread_flag = 0;
    int lexicographic_flag = 0;
    int sharedMemory_flag = 0;

    int c;
    while ((c = getopt (argc, argv, "n:tLs")) != -1) {
        switch (c)
        {
            case 'n':
                n = atoi(optarg);
                if (n <= 0) {
                    fprintf(stderr, "Invalid number of processes.\n");
                    printf("Please enter a positive integer number of processes!\n");
                    exit(1);
                }
                break;
            case 't':
                if (sharedMemory_flag == 0) thread_flag = 1;
                break;
            case 'L':
                lexicographic_flag = 1;
                break;
            case 's':
                thread_flag = 0;
                sharedMemory_flag = 1;
                break;
            default:
                fprintf(stderr, "Undefined option!\n");
                printf("Undefined option!\n");
                exit(1);
        }
    }

    vector <long long> nums;
    for (int i = optind; i < argc; i++) {
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
    printf("Before sort:\n");
    print_vector(nums);

    switch (n)
    {
        case 1:
            printf("n = 1, sort directly!\n");
            bubble_sort(nums);
            print_vector(nums);
            return 0;
        default:
            printf("n > 1, sort by subprocess or threads!\n");
            vector< vector < long long > > nums_seg;
            segment_vector(n, nums, nums_seg);

//            // check the segments
//            for (int j = 0; j < nums_seg.size(); ++j) {
//                cout << j << ":\n";
//                for (int k = 0; k < nums_seg[j].size(); ++k) {
//                    cout << nums_seg[j][k] << "\n";
//                }
//            }

            switch (thread_flag)
            {
                case 1:
                    break;
                default:
                    printf("\nstart sort by process\n");
                    sort_by_process(n, nums_seg);
                    printf("sort finished\n");
                    merge_segments(nums_seg);
                    printf("merge finished\n");
                    nums = nums_seg[0];
            }

            print_vector(nums);
            for (vector<long long> seg : nums_seg) {
                seg.clear();
            }
            nums_seg.clear();
    }

    nums.clear();
    return 0;
}

void segment_vector(int n, vector<long long > nums, vector< vector< long long > > &nums_seg) {
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

void swap(long long *a, long long *b) {
    long long temp = *a;
    *a = *b;
    *b = temp;
}

void bubble_sort(vector<long long> &nums) {
    for (auto i = 0; i < nums.size() - 1; i ++) {
        for (auto j = 0; j < nums.size() - i - 1; j ++) {
            if (nums[j] > nums[j + 1]) {
                swap(&nums[j], &nums[j + 1]);
            }
        }
    }
}

void print_vector(vector<long long> &nums) {
    for (long long i = 0; i < nums.size(); ++i) {
        printf("%lld\t%lld\n", i + 1, nums[i]);
    }
}

void sort_by_process(int n, vector< vector< long long > > &nums_seg) {
    int down[n][2];
    int up[n][2];
    int pid;
    int pipe_id;

    pipe_config(pid, pipe_id, n, down, up);
    printf("pid %d start work!\n", pid);


    // IPC in the down pipes;
    printf("IPC in the down pipes\n");
    vector<long long> seg; // store the sorted numbers
    if (pid > 0) {
        /* parent  process */
        for (int i = 0; i < n; ++i) {
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
        printf("parent write successfully!\n");
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
//        printf("child read numbers:\n");
        char buf[100];
        for(auto i = 0; i < nums_seg[pipe_id].size(); i++) {
            if (fgets(buf, sizeof(buf), child_read_file) != NULL) {
                long long val = stoll(buf);
//                printf("%lld\n", val);
                seg.push_back(val);
            }
        }

        fclose(child_read_file);
        close(down[pipe_id][0]);
        printf("child read successfully!\n");
    }
    else
    {
        fprintf(stderr, "Fork error!");
        printf("Fork error after pipe config!");
        exit(1);
    }

    // IPC in the up pipes;
    printf("IPC in the up pipes\n");
    if (pid > 0) {
        /* parent  process */
//        printf("parent process\n");
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
//                    printf("%lld\n", val);
                    num = val;
                }
            }

            fclose(parent_read_file);
            close(up[i][0]);
            printf("parent read successfully!\n");

            for (int i = 0; i < n; i++) {
                wait(NULL);
            }
        }
    }
    else if (pid == 0)
    {
        /* child process */
        // child read from the down pipe and sort the array
//        printf("child process");
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
            printf("Child write successfully!\n");
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

void pipe_config(int& pid, int& pipe_id, int n, int down[n][2], int up[n][2]) {
    for (int i = 0; i < n; ++ i) {
        pipe(up[i]);
        pipe(down[i]);
        pid = fork();

        if (pid > 0) {
            /* parent  process */
            close(down[i][0]);
            close(up[i][1]);
        } else if (pid == 0) {
            /* child process */
            pipe_id = i;
            close(down[i][1]);
            close(up[i][0]);
            break;
        } else {
            fprintf(stderr, "Fork error!");
            printf("Fork error!");
            exit(1);
        }
    }
}

vector<long long> merge(vector<long long> left, vector<long long> right) {
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

void merge_segments(vector< vector<long long > > &nums_seg) {
    while(nums_seg.size() > 1) {
        nums_seg.push_back(merge(nums_seg[0], nums_seg[1]));
        nums_seg.erase(nums_seg.begin());
        nums_seg.erase(nums_seg.begin());
    }
}

void sort_using_threads(int n, vector< vector<long long > > &nums_seg) {

}

void* bubble_sort_by_threads(void* vp) {
    auto* nums = (vector< long long >*) vp;

    for (auto i = 0; i < (*nums).size() - 1; i++) {
        for (auto j = 0; j < (*nums).size() - i - 1; j++) {
            if ((*nums)[j] > (*nums)[j + 1]) {
                swap((*nums)[j], (*nums)[j + 1]);
            }
        }
    }

    pthread_exit(0);
}