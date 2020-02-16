#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<malloc.h>
#include<string.h>

struct metadata{
	long int no_users;
	long int no_posts;
	long int no_comments;
}md;

struct userData{
	char userid[10];
	char name[20];
	char mail[30];
	char dob[10];
}ud;
//70

struct commentData{
	char postid[10];
	char userid[10];
	char commentid[10];
	char commenttxt[30];
}cd;
//60

struct postData{
	long long int size;
	char userid[10];
	char postid[10];
	char path[20];
	char postimg[3 * 1024 * 1024];
}pd;
//3145768
struct positions{
	long long int userPos;
	long long int postPos;
	long long int commentPos;
}s;

void initializePos(){
	s.userPos = 1024;
	s.postPos = 5243904;
	s.commentPos = 93324288;
}
void initializeMetadata(){
	md.no_users = 0;
	md.no_posts = 0;
	md.no_comments = 0;
}

struct user *us = NULL;
struct comment *co = NULL;
struct post *po = NULL;

void getDetails();
void createUser(){
	printf("Enter user details:");
	printf("Enter user id\n");
	scanf("%s", ud.userid);
	printf("Enter name\n");
	scanf("%s", ud.name);
	printf("Enter mail\n");
	scanf("%s", ud.mail);
	printf("Enter dob\n");
	scanf("%s", ud.dob);

	FILE *fp = fopen("output.bin", "rb+");
	md.no_users = md.no_users + 1;
	fwrite(&md, sizeof(struct metadata), 1, fp);

	long long int size = 1024 + (md.no_users - 1) * 70;
	fseek(fp, size, SEEK_SET);
	fwrite(&ud, sizeof(struct userData), 1, fp);
	printf("%s %s %s %s\n\n", ud.userid, ud.name, ud.mail, ud.dob);

	fclose(fp);
}


void createPost(){

	pd.size = 0;
	printf("\nEnter the post details\n");
	printf("Enter user id\n");
	scanf("%s", pd.userid);
	printf("Enter post id\n");
	scanf("%s", pd.postid);

	char buff[50];
	printf("Enter img path::");
	scanf("%s", pd.path);

//	printf("%s", pd.path);
	FILE *fp = fopen("testimg.jpg", "rb");

	long long int i = 0;
	while (!(feof(fp)))
	{
		fread(buff, 1, 1, fp);
		i++;
	}
	pd.size = i;
	fclose(fp);

	FILE *mounica = fopen("testimg.jpg", "rb");
	fread(pd.postimg, pd.size, 1, mounica);

	fclose(mounica);

	md.no_posts = md.no_posts + 1;

	//printf("%d", md.no_posts);

	fp = fopen("output.bin", "rb+");
	fwrite(&md, sizeof(metadata), 1, fp);

	long long int size = (5243904) + (md.no_posts - 1) * 3145768;
	fseek(fp, size, SEEK_SET);
	fwrite(&pd, sizeof(struct postData), 1, fp);

	fclose(fp);
}

void createComment(){

	printf("Enter comment details\n");
	printf("Enter post id\n");
	scanf("%s", cd.postid);
	printf("Enter user id\n");
	scanf("%s", cd.userid);

	printf("Enter comment id\n");
	scanf("%s", cd.commentid);
	printf("Enter comment\n");
	scanf("%s", cd.commenttxt);

	md.no_comments = md.no_comments + 1;
	FILE *fp = fopen("output.bin", "rb+");
	fwrite(&md, sizeof(metadata), 1, fp);

	long long int size = (93324288) + (md.no_comments - 1) * 60;
	fseek(fp, size, SEEK_SET);
	fwrite(&cd, sizeof(struct commentData), 1, fp);

	fclose(fp);

}

void updateFromFileUser(char *buff){
	
	FILE  *fp = fopen("output.bin", "rb+");
	long long int userPos = s.userPos;
	fseek(fp, userPos, SEEK_SET);

	while (ftell(fp) != 5243904){
		fread(&ud, sizeof(struct userData), 1, fp);
		if (strcmp(ud.userid, buff) == 0){
			fseek(fp, -70, SEEK_CUR);
			break;
		}
	}
	strcpy(ud.name, "changed");
	strcpy(ud.dob, "\0");
	fwrite(&ud, sizeof(struct userData), 1, fp);
	fclose(fp);
	getDetails();
}
void updateUser(){
	char buff[10];
	printf("Enter userid::\n");
	scanf("%s", buff);
	updateFromFileUser(buff);
}

void deleteFromFilePost(char *buff){
	
	FILE  *fp = fopen("output.bin", "rb+");
	long long int postPos = s.postPos;
	fseek(fp, postPos, SEEK_SET);

	while (ftell(fp) != 93324288){
		fread(&pd, sizeof(struct postData), 1, fp);
		if (strcmp(pd.postid, buff) == 0){
			fseek(fp, -3145768, SEEK_CUR);
			break;
		}
	}
	strcpy(pd.postid, "\0");
	fwrite(&pd, sizeof(struct postData), 1, fp);
	fclose(fp);
	getDetails();

}
void deletePost(){
	char buff[10];
	printf("Enter postid::\n");
	scanf("%s", buff);
	deleteFromFilePost(buff);
}

void deleteFromFileUser(char *buff){
	FILE  *fp = fopen("output.bin", "rb+");
	long long int userPos = s.userPos;
	fseek(fp, userPos, SEEK_SET);

	while (ftell(fp) != 5243904){
		fread(&ud, sizeof(struct userData), 1, fp);
		if (strcmp(ud.userid, buff) == 0){
			fseek(fp, -70, SEEK_CUR);
			break;
		}
	}
	strcpy(ud.userid, "\0");
	printf("\n%s %s %s\n\n", ud.userid, ud.name, ud.name);
	fwrite(&ud, sizeof(struct userData), 1, fp);
	fclose(fp);
	getDetails();
}

void deleteUser(){
	char buff[10];
	printf("Enter userid::\n");
	scanf("%s", buff);
	deleteFromFileUser(buff);
}

void deleteFromFileComment(char *buff){	
	FILE  *fp = fopen("output.bin", "rb+");
	long long int commentPos = 93324288;
	fseek(fp, commentPos, SEEK_SET);

	while (ftell(fp) != EOF){
		fread(&cd, sizeof(struct commentData), 1, fp);
		if (strcmp(cd.commentid, buff) == 0){
			fseek(fp, -60, SEEK_CUR);
			break;
		}
	}
	strcpy(cd.commentid, "\0");
	fwrite(&cd, sizeof(struct commentData), 1, fp);
	fclose(fp);
	getDetails();
}
void deleteComment(){
	char buff[10];
	printf("Enter commentid::\n");
	scanf("%s", buff);
	deleteFromFileComment(buff);
}

void viewPosts(){
	char buff[10];
	printf("Enter userid::\n");
	scanf("%s", buff);

	FILE  *fp = fopen("output.bin", "rb+");
	long long int postPos = 5243904;
	fseek(fp, postPos, SEEK_SET);

	long long int i = md.no_posts;
	while (i){
		fread(&pd, sizeof(struct postData), 1, fp);
		if (strcmp(pd.postid, "\0") == 0){
			i--;
			continue;
		}
		if (strcmp(pd.userid, buff) == 0){
			printf("%s %s %s \n",pd.userid,pd.postid,pd.path);
		}
		i--;
	}
//	strcpy(pd.postid, "\0");
//	fwrite(&pd, sizeof(struct postData), 1, fp);
	fclose(fp);
//	getDetails();
}

void viewComments(){
	char buff[10];
	printf("Enter postid::\n");
	scanf("%s", buff);

	FILE  *fp = fopen("output.bin", "rb+");
	long long int commentPos = 93324288;
	fseek(fp, commentPos, SEEK_SET);
	
	long long int i = md.no_comments;
	while (i){
		fread(&cd, sizeof(struct commentData), 1, fp);
		if (strcmp(cd.commentid, "\0") == 0){
			i--;
			continue;
		}
		if (strcmp(cd.postid, buff) == 0){
			printf("%s %s %s %s\n", cd.postid, cd.commentid, cd.userid, cd.commenttxt);
		}
		i--;
	}
	fclose(fp);
}


void download(){
	char buff[10];
	printf("Enter postid::\n");
	scanf("%s", buff);

	FILE  *fp = fopen("output.bin", "rb+");
	FILE *fp2 = fopen("download.jpg", "wb");
	long long int postPos = 5243904;
	fseek(fp, postPos, SEEK_SET);

	long long int i = md.no_posts;
	while (i){
		fread(&pd, sizeof(struct postData), 1, fp);
		if (strcmp(pd.postid, buff) == 0){
			//printf("%s %s %s \n", pd.userid, pd.postid, pd.path);
			fwrite(pd.postimg, pd.size, 1, fp2);
			break;
		}
		i--;
	}
	fclose(fp2);
	fclose(fp);
}
void getDetails(){
	initializePos();
	FILE *fp = fopen("output.bin", "rb");
	FILE *fp1 = fopen("output.bin", "rb");
	FILE *fp2 = fopen("output.bin", "rb");
	fread(&md, sizeof(metadata), 1, fp);

	long int i = md.no_users;
	long int j = md.no_posts;
	long int k = md.no_comments;

	long long int userPos = s.userPos;
	long long int postPos = s.postPos;
	long long int commentPos = s.commentPos;

	fseek(fp, userPos, SEEK_SET);
	fseek(fp1, postPos, SEEK_SET);
	fseek(fp2, commentPos, SEEK_SET);


	struct user *u = us;
	struct post *p = po;
	struct comment *c;
	//printf("User details\n");
	printf("reading.............\n\n\n");
	printf("UserDetails\n\n\n");
	while (i){

		fread(&ud, sizeof(struct userData), 1, fp);
		if (strcmp(ud.userid, "\0") == 0){
			i--;
			continue;
		}
		printf("%s %s %s %s \n", ud.userid, ud.name, ud.mail, ud.dob);
		i--;
	}
	printf("\n\nPost Details\n\n");
	while (j){
		fread(&pd, sizeof(struct postData), 1, fp1);
		if (strcmp(pd.postid, "\0") == 0){
			j--;
			continue;
		}
		printf("%s %s %s %d\n", pd.userid, pd.postid,pd.path, pd.size);

		FILE *fpp = fopen("midnight.jpg", "wb");
		for (long long int i = 0; i < pd.size; i++){
			fwrite(&(pd.postimg[i]), 1, 1, fpp);
		}
		fclose(fpp);

		j--;
	}
	printf("\n\nComment Details\n\n");
	while (k){
		fread(&cd, sizeof(struct commentData), 1, fp2);

		if (strcmp(cd.commentid, "\0") == 0){
			k--;
			continue;
		}
		printf("%s %s %s %s\n", cd.postid, cd.userid, cd.commentid, cd.commenttxt);
		k--;
	}


	fclose(fp);
	fclose(fp1);
	fclose(fp2);
}

int main(){
	initializeMetadata();
	int choice;
	while (1){
		printf("\n\n1.Enter user\n");
		printf("2.Enter post\n");
		printf("3.Enter comment\n");
		printf("4.Update User\n");
		printf("5.Delete User\n");
		printf("6.Delete Post\n");
		printf("7.Delete Comment\n");
		printf("8.View posts\n");
		printf("9.Download posts\n");
		printf("10.View comments\n");
		printf("11.EXIT\n");
		scanf("%d", &choice);

		if (choice == 1){
			createUser();
		}
		else if (choice == 2){
			createPost();
		}
		else if (choice == 3){
			createComment();
		}
		else if (choice == 4){
			updateUser();
		}
		else if (choice == 5){
			deleteUser();
		}
		else if (choice == 6){
			deletePost();
		}
		else if (choice == 7){
			deleteComment();
		}
		else if (choice == 8){
			viewPosts();
		}
		else if (choice == 9){
			download();
		}
		else if (choice == 10){
			viewComments();
		}
		else if (choice == 11)
			break;
	}
	//	fetchDetails();
	//	movDetails();
	getDetails();


	_getch();
}