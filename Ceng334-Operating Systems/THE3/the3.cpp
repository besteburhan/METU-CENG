#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream>
#include <string.h>
#include <cmath>
#include "ext21.h"

#define BASE_OFFSET 1024
#define EXT2_BLOCK_SIZE 1024
#define IMAGE "image.img"

typedef unsigned char bmap;
#define __NBITS (8 * (int) sizeof (bmap))
#define __BMELT(d) ((d) / __NBITS)
#define __BMMASK(d) ((bmap) 1 << ((d) % __NBITS))
#define BM_SET(d, set) ((set[__BMELT (d)] |= __BMMASK (d)))
#define BM_CLR(d, set) ((set[__BMELT (d)] &= ~__BMMASK (d)))
#define BM_ISSET(d, set) ((set[__BMELT (d)] & __BMMASK (d)) != 0)

unsigned int block_size = 0;
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)
using namespace std;
ext2_super_block super;

int fd;
void getSuper( ){
	lseek(fd,BASE_OFFSET,SEEK_SET);
	read(fd,&super,sizeof(super));
	block_size = 1024 << (super).s_log_block_size;
}
void getGroup( int bgroupId , struct ext2_group_desc * group){
	lseek(fd,BASE_OFFSET+block_size+sizeof(*group)*bgroupId,SEEK_SET);
	read(fd,group,sizeof(*group));
}
void selectEmptyInode( int *allocatedInode, int* selectedFromGroupid){
	for(int i=0; i< (super.s_blocks_count-1)/super.s_blocks_per_group+1;i++){
		ext2_group_desc tempGroup ;
		getGroup(i,&tempGroup);
		//*allocatedInode = super.s_first_ino;
		int addrInodeBitmap;
		addrInodeBitmap = tempGroup.bg_inode_bitmap;
		bmap inodeBmap[block_size];
		lseek(fd,BLOCK_OFFSET(addrInodeBitmap),SEEK_SET);
		read(fd,inodeBmap,block_size);
		for(int j=0; j < super.s_inodes_per_group ; j++){

			if(!BM_ISSET(j,inodeBmap)){
				(*allocatedInode)=j+1;
				*selectedFromGroupid=i;
				tempGroup.bg_free_inodes_count-=1;
				super.s_free_inodes_count = super.s_free_inodes_count-1;
				break;
			}
		}
		if(*allocatedInode!=-1){
			BM_SET(*allocatedInode-1,inodeBmap);
			lseek(fd,BLOCK_OFFSET(addrInodeBitmap),SEEK_SET);
			write(fd,inodeBmap,block_size);//inode bitmap updating
			
			lseek(fd,BASE_OFFSET,SEEK_SET);
			write(fd,&super,block_size);//super block updating

			lseek(fd,BASE_OFFSET+block_size+sizeof(tempGroup)*i,SEEK_SET);
			write(fd,&tempGroup,sizeof(tempGroup));
			break;	
			
		}
		else
			continue;
	}
}
void fillInodeInfo(char *fileName, ext2_inode *inodeForNewFile){
	struct stat sb;
	if (stat(fileName, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
	inodeForNewFile->i_mode = sb.st_mode;
	inodeForNewFile->i_uid = 0xFFFF & sb.st_uid;
	inodeForNewFile->i_size = sb.st_size;
	inodeForNewFile->i_atime = sb.st_atime;
	inodeForNewFile->i_mtime = sb.st_mtime;
	inodeForNewFile->i_gid = 0xFFFF & sb.st_gid;
	inodeForNewFile->i_links_count = 1;

	int howManyDataBlock = ((inodeForNewFile->i_size-1)/block_size+1);
	unsigned int iblocks = howManyDataBlock*block_size/512;
	inodeForNewFile->i_blocks = iblocks;
}
void fillDataBlocks(char *fileName,int howManyDataBlock,int *dataBlocks, size_t sizeOfFile,ext2_inode * inode){
	int k=howManyDataBlock;
	int dat=0;
	size_t sz =sizeOfFile;
	int inFile = open(fileName,O_RDONLY);
	for(int i=0; i< super.s_blocks_count/super.s_blocks_per_group+1;i++){
		ext2_group_desc tempGroup;
		getGroup(i,&tempGroup);
		bool changedBmap=false;
		int firstDataBlock = tempGroup.bg_inode_table+ceil(super.s_inodes_per_group*super.s_inode_size/(float)block_size) - i*super.s_blocks_per_group;//index in block group()block bitmap direk okuyabilirsin
		bmap blockBmap[super.s_blocks_per_group];
		lseek(fd,BLOCK_OFFSET(tempGroup.bg_block_bitmap),SEEK_SET);
		read(fd,blockBmap,block_size);
		int t=0;
		for(int j = firstDataBlock ; (k!=0 && j < super.s_blocks_per_group) ; j++){
			if(!BM_ISSET(j,blockBmap)){
				dataBlocks[howManyDataBlock-k]=j+1+i*super.s_blocks_per_group;
				inode->i_block[t]=j+1+i*super.s_blocks_per_group;t++;
				super.s_r_blocks_count = super.s_r_blocks_count+1;
				super.s_free_blocks_count = super.s_free_blocks_count-1;
				tempGroup.bg_free_blocks_count= tempGroup.bg_free_blocks_count-1;
				BM_SET(j,blockBmap);
				changedBmap=true;
				k--;
			}
		}
		if(changedBmap){
			lseek(fd,BLOCK_OFFSET(tempGroup.bg_block_bitmap),SEEK_SET);
			write(fd,blockBmap,block_size);

			
			
			char buf[block_size];
			
			for(; dat<howManyDataBlock-k ; dat++ ){
				lseek(inFile,dat*block_size,SEEK_SET);
				if(dat==howManyDataBlock-k-1)
					read(inFile,buf,sz);
				else {
					read(inFile, buf, block_size);
					sz-=block_size;
				}
				
				if(howManyDataBlock < 12 ){
						lseek(fd,BLOCK_OFFSET(dataBlocks[dat]),SEEK_SET);
						write(fd,buf,sz);
				}
				else{
					// indirect blocks
				}
			}
			lseek(fd,BASE_OFFSET,SEEK_SET);
			write(fd,&super,block_size);

			lseek(fd,BASE_OFFSET+block_size+sizeof(tempGroup)*i,SEEK_SET);
			write(fd,&tempGroup,sizeof(tempGroup));
		}

		
		if(k==0) {
			close (inFile);
			break;
		}
	}
}

void createDirEntry( int allocatedInode, char * argv_2 ,unsigned short newEntryRecLen ,ext2_dir_entry* newDirEntry){
	string argv_path = argv_2;
	unsigned char newEntryNameLen = (unsigned char)(argv_path.length());
	unsigned char newFileType = (unsigned char ) 1;
	newDirEntry->inode = allocatedInode;
	newDirEntry->rec_len = newEntryRecLen;
	newDirEntry->name_len = newEntryNameLen;
	newDirEntry->file_type = newFileType;
	for(int i=0;i<(int )argv_path.length();i++){
		newDirEntry->name[i]=argv_path[i];
	}
}

void addSourceFileToTargetInode(int argv_inode,char * argv_2){
	ext2_group_desc group;
	int blockGroup_of_inode = (argv_inode-1) / super.s_inodes_per_group; 
	argv_inode = argv_inode - blockGroup_of_inode*super.s_inodes_per_group;
	getGroup(blockGroup_of_inode,&group);
	ext2_inode targetInode;
	lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+(argv_inode-1)*super.s_inode_size, SEEK_SET);
	read(fd, &targetInode, sizeof(targetInode)); 
	//allocate new inode
	int allocatedInode =-1;
	int selectedFromGroupid=-1;
	selectEmptyInode( &allocatedInode,&selectedFromGroupid);
	ext2_group_desc grp;
	getGroup(selectedFromGroupid,&grp);
	ext2_inode inodeForNewFile;
	lseek(fd, BLOCK_OFFSET(grp.bg_inode_table)+(allocatedInode-1)*super.s_inode_size,SEEK_SET );
	read(fd,&inodeForNewFile,sizeof(inodeForNewFile));
	fillInodeInfo(argv_2, &inodeForNewFile);
	
		
	struct stat sb;
	if (stat(argv_2, &sb) == -1) {
	    perror("stat");
	    exit(EXIT_FAILURE);
	}
	size_t sizeOfFile = sb.st_size;
	int howManyDataBlock = ((sizeOfFile-1)/block_size)+1;
	int dataBlocks[howManyDataBlock];
	fillDataBlocks(argv_2,howManyDataBlock,dataBlocks,sizeOfFile,&inodeForNewFile);

	cout << allocatedInode <<" " ; 
	for(int i=0; i<howManyDataBlock;i++){
		if(i==howManyDataBlock-1)
			cout << dataBlocks[i]<<endl;  // for output
		else 
			cout << dataBlocks[i] <<"," ;  // for output
	}
	lseek(fd,BLOCK_OFFSET(grp.bg_inode_table)+(allocatedInode-1)*super.s_inode_size,SEEK_SET);
	write(fd,&inodeForNewFile,sizeof(inodeForNewFile));
	//add dirEntry to directory 	    
	unsigned char totalDataBlock[block_size];
	int num_of_dataBlck_dirEntry= ceil(targetInode.i_size/(float)block_size);
	lseek(fd, BLOCK_OFFSET(targetInode.i_block[num_of_dataBlck_dirEntry-1]), SEEK_SET);
	read(fd, totalDataBlock, block_size);  
	int size = 0;   
	ext2_dir_entry * entry = (struct ext2_dir_entry *) totalDataBlock;    
	size += entry->rec_len;
		while(size < block_size) {
			
		    char file_name[EXT2_NAME_LEN+1]; memcpy(file_name, entry->name, entry->name_len);
		    file_name[entry->name_len] = 0;  
		    entry = (struct ext2_dir_entry *)((char *)entry + entry->rec_len);
		    size += entry->rec_len;
		}
		char file_name[EXT2_NAME_LEN+1]; memcpy(file_name, entry->name, entry->name_len);
		    file_name[entry->name_len] = 0; 
		int nameLen = (int) entry->name_len;
		size -= entry->rec_len;
		if(nameLen % 4!=0) nameLen += 4-(nameLen%4);
				
		int szOfEntry = 8+ (int) nameLen;
		unsigned short newEntryRecLen = entry->rec_len - szOfEntry; 
		
		unsigned short actualRecLen= (unsigned short) szOfEntry;
		unsigned short emptyLen = entry->rec_len - actualRecLen;
		entry->rec_len = actualRecLen;

		if(emptyLen >= newEntryRecLen){
			lseek(fd,BLOCK_OFFSET(targetInode.i_block[num_of_dataBlck_dirEntry-1])+size,SEEK_SET);
			write(fd,entry,actualRecLen);
			ext2_dir_entry newDirEntry;
			int realAllocatedInode= selectedFromGroupid*super.s_inodes_per_group+allocatedInode;
			createDirEntry( realAllocatedInode,argv_2, newEntryRecLen  ,&newDirEntry);

			entry = (struct ext2_dir_entry *)((char *)entry + entry->rec_len);
			lseek(fd,BLOCK_OFFSET(targetInode.i_block[num_of_dataBlck_dirEntry-1])+size+actualRecLen,SEEK_SET);
			write(fd,&newDirEntry,newEntryRecLen);
			
		}
		else{

			unsigned int allocatedDatablock=0;
			for(int i=0; i< super.s_blocks_count/super.s_blocks_per_group+1;i++){
				ext2_group_desc tempGroup ;
				getGroup(i,&tempGroup);

				int firstDataBlock = tempGroup.bg_inode_table+ceil(super.s_inodes_per_group*super.s_inode_size/(float)block_size) - i*super.s_blocks_per_group -1;//index in block group

				bmap blockBmap[super.s_blocks_per_group];
				lseek(fd,BLOCK_OFFSET(tempGroup.bg_block_bitmap),SEEK_SET);
				read(fd,blockBmap,block_size);
				for(int j = firstDataBlock ; j < super.s_blocks_per_group ; j++){
					if(!BM_ISSET(j,blockBmap)){
						allocatedDatablock = j;
						super.s_r_blocks_count = super.s_r_blocks_count+1;
						super.s_free_blocks_count = super.s_free_blocks_count-1;
						tempGroup.bg_free_blocks_count= tempGroup.bg_free_blocks_count-1;
						BM_SET(j,blockBmap);
					}
				}
				lseek(fd,BLOCK_OFFSET(tempGroup.bg_block_bitmap),SEEK_SET);
				write(fd,blockBmap,block_size);
	
				lseek(fd,BASE_OFFSET,SEEK_SET);
				write(fd,&super,block_size);

				lseek(fd,BASE_OFFSET+block_size+sizeof(tempGroup)*i,SEEK_SET);
				write(fd,&tempGroup,sizeof(tempGroup));

				if(allocatedDatablock != 0) break;
			}

			targetInode.i_block[num_of_dataBlck_dirEntry]=allocatedDatablock;
			lseek(fd, BLOCK_OFFSET(group.bg_inode_table)+(argv_inode-1)*super.s_inode_size, SEEK_SET);
			write(fd, &targetInode, sizeof(targetInode)); 

			ext2_dir_entry newDirEntry;
			int realAllocatedInode= selectedFromGroupid*super.s_inodes_per_group+allocatedInode;
			createDirEntry( realAllocatedInode,argv_2, block_size  ,&newDirEntry); 
			lseek(fd, BLOCK_OFFSET(targetInode.i_block[num_of_dataBlck_dirEntry]), SEEK_SET);
			write(fd,&newDirEntry,sizeof(ext2_dir_entry));


		}

}

int main(int argc, char *argv[])
{	
    struct ext2_group_desc group;
    int argv_inode;

    fd = open(argv[1],O_RDWR);
	getSuper();
	if(argv[3][0]!='/'){ //if argv[3] is inode(int)
		argv_inode = atoi(argv[3]);
		addSourceFileToTargetInode(argv_inode,argv[2]);
	}
	else{
		ext2_group_desc group;
		vector<string> vectorPath;
		string argv_path = argv[3];
		string delimiter = "/";

		size_t pos = 0;
		string token;
		while ((pos = argv_path.find(delimiter)) <= argv_path.length()-1 && argv_path.length()!=0) {
		    token = argv_path.substr(0, pos);
		    if(token.length()!=0){
			    vectorPath.push_back(token);
			    
			}
			argv_path.erase(0, pos + delimiter.length());
		}
		if(argv_path.length()!=0)
			vectorPath.push_back(argv_path);
		if(vectorPath.size()==0){
			addSourceFileToTargetInode(2,argv[2]);
		}
		else{
			getGroup(0,&group);
			ext2_inode targetInode;
			lseek(fd,BLOCK_OFFSET(group.bg_inode_table)+super.s_inode_size,SEEK_SET);
			read(fd,&targetInode,sizeof(targetInode));

			for(int i=0; i<vectorPath.size();i++){
				string temp = vectorPath[i];
				unsigned char totalDataBlock[block_size];
				int numofdataBlck= ceil(targetInode.i_size/(float)block_size);
				for(int j=0 ; j<numofdataBlck ; j++){
					lseek(fd, BLOCK_OFFSET(targetInode.i_block[j]), SEEK_SET);
					read(fd, totalDataBlock, block_size);
					int size = 0;   
					ext2_dir_entry * entry = (struct ext2_dir_entry *) totalDataBlock;    
					size += entry->rec_len;
					char file_name[EXT2_NAME_LEN+1]; 
					memcpy(file_name, entry->name,(int) entry->name_len);
					file_name[(int)entry->name_len] = 0;  
					string nameEntry=file_name;
					while(size < block_size && nameEntry.compare(vectorPath[i])!=0) {
						     
						    entry = (struct ext2_dir_entry *)((char *)entry + entry->rec_len);
						    char file_name[EXT2_NAME_LEN+1]; 
							memcpy(file_name, entry->name,(int) entry->name_len);
							file_name[(int)entry->name_len] = 0;
						    nameEntry = file_name;
						    size += entry->rec_len;
						    
						}
						if(nameEntry.compare(vectorPath[i])==0){//same name

							argv_inode = entry->inode;
							if(i==vectorPath.size()-1){
								addSourceFileToTargetInode(argv_inode,argv[2]);
								break;
							}else{
								int blockGr = (argv_inode-1)/super.s_inodes_per_group;
								argv_inode = argv_inode-super.s_inodes_per_group*blockGr;
								ext2_group_desc grp;
								getGroup(blockGr,&grp);
								lseek(fd,BLOCK_OFFSET(grp.bg_inode_table)+super.s_inode_size*(argv_inode-1),SEEK_SET);
								read(fd,&targetInode,sizeof(targetInode));
								break;
							}	
						}
						else{
							continue;
						}
						
				}                 

			}
		}
	}
	close(fd);

    return 0;
    
}


