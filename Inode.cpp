#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <fstream>

using namespace std;

class superblock
{
    public:
        int nodes;
        int blocks;
        int block_size;
};

class inode
{
    public:
        int size;
        int start;
        char name[16];
};

class disk_block
{
    public:
        int next_block;
        char data[600000];
};


superblock sb;
inode *inodes;
disk_block *dbs;

struct info_about_files
{
    int mode;
    int fd;
};

struct meta
{
    unordered_map<string,int> fds;
};

unordered_set<string> disk_names;
unordered_map<string,int> file_names; // load files at mounting
unordered_map<string,info_about_files> open_files; // opened files [disk,structure]
unordered_map<string,meta> all_disks;


void create_filesystem()
{
    sb.nodes = 10;
    sb.blocks = 865;
    sb.block_size = sizeof(disk_block);

    inodes = new inode[sb.nodes];
    for(int i=0;i<sb.nodes;i++)
    {
        inodes[i].size = -1;
        inodes[i].start = -1;
        strcpy(inodes[i].name,"");
        // inodes[i].name = "";
    }
     
    dbs = new disk_block[sb.blocks];
    for(int i=0;i<sb.blocks;i++)
    {
        dbs[i].next_block = -1;
        // dbs[i].data = "";
        // string q ="\0";
        strcpy(dbs[i].data,"\0");
    }
}

void mount_filesystem(string name)
{
    FILE *fp;
    fp = fopen(name.c_str(),"r");

    fread(&sb, sizeof(superblock),1,fp);

    inodes = new inode[sb.nodes];
    dbs = new disk_block[sb.blocks];

    fread(inodes, sizeof(inode), sb.nodes, fp);
    fread(dbs, sizeof(disk_block), sb.blocks, fp);

    fclose(fp);

    for(int i=0;i<sb.nodes;i++)
    {
        if(strlen(inodes[i].name) != 0)
        {
            string s;
            for(int j=0;j<16;j++)
            {
                if(inodes[i].name[j] != '\0')
                s.push_back(inodes[i].name[j]);
                else 
                break;
            }

            file_names[s]=i;
            // s.push
        }
    }
}

void sync_filesystem(string fname)
{
    FILE *fp;
    fp = fopen(fname.c_str(),"w+");
    fwrite(&sb, sizeof(superblock),1,fp);

    for(int i=0;i<sb.nodes;i++)
    {
        fwrite(&(inodes[i]), sizeof(inode),1,fp);
    }

    for(int i=0;i<sb.blocks;i++)
    {
        fwrite(&(dbs[i]), sizeof(disk_block),1,fp);
    }

    fclose(fp);
}


int make_file(string name)
{
    int ians = -1;
    for(int i=0;i<sb.nodes;i++)
    {
        if(inodes[i].start == -1){
        ians = i;
        break;
        }
    }

    int bans = -1;
    for(int i=0;i<sb.blocks;i++)
    {
        if(dbs[i].next_block == -1){
        bans = i;
        break;
        }
    }


    inodes[ians].start = bans;
    dbs[bans].next_block = -2;

    name.push_back('\0');
    strcpy(inodes[ians].name,name.c_str());

    return ians;
}


// void shorten_file(int bn)
// {
//     if(dbs[bn].next_block >= 0)
//     shorten_file(dbs[bn].next_block);
//     dbs[bn].next_block = -1;
// }

// void set_filesize(int filenum, int size)
// {
//     int x1 = size+512-1;

//     int x2 = x1/512;

//     int bn = inodes[filenum].start;
//     x2--;

//     while(x2)
//     {
//         int x3 = dbs[bn].next_block;
//         if(x3 == -2)
//         {
//             int bans = -1;
//             for(int i=0;i<sb.blocks;i++)
//             {
//                 if(dbs[i].next_block == -1){
//                 bans = i;
//                 break;
//                 }
//             }

//             dbs[bn].next_block = bans;
//             dbs[bans].next_block = -2;
//         }
//         bn = dbs[bn].next_block;
//         x2--;
//     }

//     shorten_file(bn);
//     dbs[bn].next_block = -2;
// }



int flag=1;

void read_file(int fd)
{
    int block = inodes[fd].start;
    while(1){
    char *buffer = dbs[block].data;
    int i=0;
    while(*buffer != '\0' && i != 600000){
    cout<<*buffer;
    i++;
    buffer++;
    }
    if(*buffer == '\0')
    break;
    block = dbs[block].next_block;
    }

    cout<<endl;
}


void write_file(int fd)
{
    int block = inodes[fd].start;
    // cout<<"Enter data: ";
    // string s;
    // cin>>s;
    char buffer1[600000];
    // char c ;
    // cin>>c;
    // buffer[0] = c;
    // int i =1;
    // while((c = cin.get()) != '\n')
    // {
    //     buffer[i] = c;
    //     // c = cin.get();
    //     i++; // manage 511 512 512.............

    //     if(i == 512)
    //     {
    //         // allocate new block to it and write there
    //         strcpy(dbs[block].data, buffer);
    //         //clear remining entries of buffer---> it is cleared i think
    //         int bans = -1;
    //         for(int x=0;x<sb.blocks;x++)
    //         {
    //             if(dbs[x].next_block == -1){
    //             bans = x;
    //             break;
    //             }
    //         }
    //         if(bans == -1)
    //         {
    //             cout<<"Disk is full";
    //             return;
    //         }
    //         cout<<bans;

    //         dbs[block].next_block = bans;
    //         block = bans;
    //         dbs[bans].next_block = -2;
    //         i=0;
    //     }
    // }
    // s.push_back('\0');

    // char c;
    // cin>>c;
    // string st;
    // st.push_back(c);
    // while((c = cin.get()) != '\n')
    // st.push_back(c);

    // string s1; 
    // s1.push_back(st[0]);
    // int i;
    // for(i=1;i<st.size();i++)
    // {
    //     s1.push_back(st[i]);
    //     // i++;
    //     if(i % 512 == 0)
    //     {
    //         strcpy(dbs[block].data, s1.c_str());
    //         int bans = -1;
    //         for(int x=0;x<sb.blocks;x++)
    //         {
    //             if(dbs[x].next_block == -1){
    //             bans = x;
    //             break;
    //             }
    //         }
    //         if(bans == -1)
    //         {
    //             cout<<"Disk is full";
    //             return;
    //         }
    //         cout<<bans;

    //         dbs[block].next_block = bans;
    //         block = bans;
    //         dbs[bans].next_block = -2;
    //         // i=0;
    //         s1.clear();
    //     }
    // }

        // i=0;
    int i=0;
    cout<<"Enter data: ";
    char c ;
    cin>>c;
    buffer1[i] = c;
    i++;
    // i =1;
    while((c = cin.get()) != '\n')
    {
        buffer1[i] = c;
        // c = cin.get();
        i++;

        if(i == 600000)
        {
            // allocate new block to it and write there
            strcpy(dbs[block].data, buffer1);
            if(dbs[block].next_block == -2){
            int bans = -1;
            for(int x=0;x<sb.blocks;x++)
            {
                if(dbs[x].next_block == -1){
                bans = x;
                break;
                }
            }
            if(bans == -1)
            {
                cout<<"Disk is full";
                return;
            }

            dbs[block].next_block = bans;
            // block = bans;
            dbs[bans].next_block = -2;
            }
            block = dbs[block].next_block;
            i=0;
        }
    }
    if(i != 600000)
    {
        buffer1[i] = '\0';
        strcpy(dbs[block].data, buffer1);
    }

    // if(i != 512)
    // {
    //     s1.push_back('\0');
    //     strcpy(dbs[block].data, s1.c_str());
    // }
}

bool is_open(int fd, int mode, int given_mode)
{
    if(fd == -1)
    {
        cout<<"Open file to do perticular operation on file";
        return false;
    }

    if(mode != given_mode)
    {
        cout<<"The mode in which you opened file can not do this operation";
        return false;
    }

    return true;
}

void append_file(int fd)
{
    int block = inodes[fd].start;
    int i=0;
    char buffer1[600000];
    // char buffer1[512];
    while(block >= 0){ // implement multiple block thing too!
    i=0;
    char *buffer = dbs[block].data;
    int last = dbs[block].next_block;
    while(*buffer != '\0' && i != 599999 && last == -2){
    // i++;
    // cout<<*buffer;
    buffer1[i] = *buffer;
    buffer++;
    i++;
    }
    if(*buffer == '\0')
    break;
    block = dbs[block].next_block;
    }

    // implement write part too!
    // char c ;
    // cin>>c;
    // buffer1[i] = c;
    // int i =1;
    // i=0;
    // i=0;
    cout<<"Enter data: ";
    char c ;
    cin>>c;
    buffer1[i] = c;
    i++;
    // i =1;
    while((c = cin.get()) != '\n')
    {
        buffer1[i] = c;
        // c = cin.get();
        i++;

        if(i == 600000)
        {
            // allocate new block to it and write there
            strcpy(dbs[block].data, buffer1);
            if(dbs[block].next_block == -2){
            int bans = -1;
            for(int x=0;x<sb.blocks;x++)
            {
                if(dbs[x].next_block == -1){
                bans = x;
                break;
                }
            }
            if(bans == -1)
            {
                cout<<"Disk is full";
                return;
            }

            dbs[block].next_block = bans;
            // block = bans;
            dbs[bans].next_block = -2;
            }
            block = dbs[block].next_block;
            i=0;
        }
    }
    if(i != 600000)
    {
        buffer1[i] = '\0';
        strcpy(dbs[block].data, buffer1);
    }
}


void print_menu()
{
    cout<<"Enter your choice: "<<endl;
    cout<<"1. Create file\n2. Open file\n3. Read file\n4. Write file\n5. Append file\n6. Close file\n7. Delete file\n8. List of files\n9. List of opened files\n10. Unmount"<<endl;
    return;
}

int go_back=0;


void section2(string s)
{
    while(1)
    {
        int x;
        print_menu();
        cin>>x;
        int f_d;

        if(x == 1)
        {
            cout<<"Enter file name: ";
            string f;
            cin>>f;
            if(file_names.find(f) != file_names.end())
            {
                cout<<"File already exists, use other name";
                return;
            }

            cout<<"File created"<<endl;
            f_d = make_file(f);
            file_names[f] = f_d;

            // add file names and fd

        }

        else if( x == 2)
        {
            cout<<"Enter file name: ";
            string f;
            cin>>f;

            if(file_names.find(f) == file_names.end())
            {
                cout<<"No such file exists: "<<endl;
                return;
            }

            else if(open_files.find(f) != open_files.end())
            {
                cout<<"FIle is already opened";
                cout<<endl;
                return;
            }
            cout<<"Enter mode in which you want to open the file: "<<endl;
            cout<<"0: read mode\n1: write mode\n2: append mode"<<endl;
            int mode;
            cin>>mode;

            // sync_files(s);
            // mount_fs(s);

            // open_files[f].fd = file_names[f];
            // open_files[f].mode = mode;
            open_files.insert({f,{0,0}});
            open_files[f].fd = file_names[f];
            open_files[f].mode = mode;

            string mode_str;
            if(mode == 0)
            mode_str = "read mode";
            else if(mode == 1)
            mode_str = "write mode";
            else if(mode == 2)
            mode_str = "append mode";
            else
            {
                cout<<"Enter correct mode"<<endl;
                break;
            }
            cout<<"File descriptor: "<<open_files[f].fd<<"\nMode of file opened: "<<mode_str<<endl;
            // else
            // return;
        }

        else if(x == 3)
        {
            int fd;
            cout<<"Enter file descriptor: ";
            cin>>fd; // have error checking, see file is opened or not
            // clear doubt for mode as if not opened with write mode then can we write?

            int ok=-1,mode;
            for(auto it : open_files)
            {
                if(it.second.fd == fd)
                {
                    ok = 0;
                    mode = it.second.mode;
                }
            }

            // if(ok == -1)
            // {
            //     cout<<"Open file to do perticular operation on file";
            //     return;
            // }

            // if(mode != 0)
            // {
            //     cout<<"The mode in which you opened file can not do this operation";
            //     return;
            // }
            // sync_files(s);
            // mount_fs(s);
            if(is_open(fd,mode,0))
            read_file(fd);
            else
            return;
        }

        else if(x == 4)
        {
            int fd;
            cout<<"Enter file descriptor: ";
            cin>>fd; // have error checking, see file is opened or not
            string f;

            int ok=-1,mode;
            for(auto it : open_files)
            {
                if(it.second.fd == fd)
                {
                    ok = 0;
                    f = it.first;
                    mode = it.second.mode;
                }
            }

            // if(ok == -1)
            // {
            //     cout<<"Open file to do perticular operation on file";
            //     return;
            // }

            // if(mode != 1)
            // {
            //     cout<<"The mode in which you opened file can not do this operation";
            //     return;
            // }
            // sync_files(f);
            // mount_fs(f);
            if(is_open(fd,mode,1))
            write_file(fd);
            else
            return;
        }

        else if(x == 5)
        {
            int fd;
            cout<<"Enter file descriptor: ";
            cin>>fd; // have error checking, see file is opened or not

            int ok=-1,mode;
            for(auto it : open_files)
            {
                if(it.second.fd == fd)
                {
                    ok = 0;
                    mode = it.second.mode;
                }
            }
            if(is_open(fd,mode,2))
            append_file(fd);
            else
            return;        
        }

        else if(x == 6)
        {
            int fd;
            cout<<"Enter file descriptor: ";
            cin>>fd;
            int f=0;

            for(auto it : open_files)
            {
                if(it.second.fd == fd)
                {
                    f = 1;
                    string close = it.first;
                    open_files.erase(close);
                    cout<<"File is closed";
                    break;
                }
            }

            if(f == 0)
            cout<<"File is already closed";
            cout<<endl;
        }

        else if(x == 7)
        {
            string f;
            cout<<"Enter file to delete: ";
            cin>>f;

            if(file_names.find(f) == file_names.end())
            {
                cout<<"File does not exists";
                return;
            }

            int fd;
            fd = file_names[f];
            file_names.erase(f);

            if(open_files.find(f) != open_files.end())
            open_files.erase(f);

            //delete file
            //start file with null character
            int block = inodes[fd].start;
            inodes[fd].start = -1; 
            strcpy(inodes[fd].name,"");

            while(block != -2)
            {
                int x = dbs[block].next_block;
                dbs[block].next_block = -1;
                block = x;
            } 
            // dbs[block].next_block = -1;

            cout<<"File is deleted";
            cout<<endl;
        }

        else if(x == 8)
        {
            cout<<"Files: "<<endl;
            for(auto it : file_names)
            cout<<it.first<<endl;
        }

        else if(x == 9)
        {
            cout<<"Currently opened files: "<<endl;
            for(auto it : open_files)
            {
                cout<<it.first<<endl;
            }
        }

        else if(x == 10)
        {
            sync_filesystem(s);
            file_names.clear();
            open_files.clear();
            go_back =1;
            return;
        }
    }
}


int main()
{
    while(1)
    {
        go_back=0;
        cout<<"1. Create Disk"<<endl<<"2. Mount Disk"<<endl<<"3. Exit "<<endl;
        int x;
        cin>>x;
        if(x == 1)
        {
            cout<<"Enter disk name: ";
            string s;
            cin>>s;

            if(disk_names.find(s) != disk_names.end())
            {
                cout<<"Disk with this name already exists"<<endl;
            }
            disk_names.insert(s);
            create_filesystem();
            // print_fs();
            sync_filesystem(s);
        }

        else if(x == 2)
        {
            cout<<"Enter name of disk to mount: ";
            string s;
            cin>>s;

            if(disk_names.find(s) == disk_names.end())
            {
                cout<<"No such disk exists"<<endl;
                break;
            }
            else
            {
                mount_filesystem(s);
                while(flag){
                // print_menu();
                if(go_back == 1)
                break;
                section2(s);
                }
                // print_fs();
            }
        }

        else if(x == 3)
        {
            break;
        }

        else{
            cout<<"Enter correct command"<<endl;
        }
    }
}