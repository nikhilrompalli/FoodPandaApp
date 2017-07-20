#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

struct node
{
	char name[50];
	int cost;
	struct node *rest;
	struct node *items;
};

int checkLogin(char *,char *,char *);			//validation of  username and password
void login();									//interface to type username and password
void getRestaurentArea(char *,char*);			//compares his city with set of cities name in restaurent.txt file if found then it displays welecome message 
void userRegister();							//registertion for new customer
void loadLogIn(char *);							//loades the customer email into logedIn file,to create session
void getLocation();								//displays some set of cities and provieds option to select one from them
int userAccessMenu(char*);						//provides interface to select options like 1.resuarent lists  2.track orders  3.log out
int checkExist(char*);							//checks whether user is already exits or not while regestring new user
int selectItem(struct node *,char*);			//provides interface to select items and placing orders
void displayMenu(struct node *,char*);			//displays menu of all items in selected restautent
void displayretaurents(struct node *);			//displays all restaurants in selected city
void selectRest(struct node *,char*);			//after selecting restaurent number,based on the number travese to that particular restuarent usinf linked list
struct node* create(struct node*,char*);		//creates new node and adds to linked list
void restaurentlist(FILE *,char*);				//takes restuarent data from the restuarent.txt file and loads into linked List 
void findNameByEmail(char*);					//takes username(Email) as input and gives name of the customer
int trackOrders(char *);						//gives history all orders list
void logOut();									//provides logout interface to customer


char* location,*currtRest;


int main()
{
		int ch;
		FILE* fp;
		char buff[255];
		printf("\n\n-----------------------------WELCOME TO FOOD PANDA---------------------------------\n");
		fp=fopen("logedInUser.txt","r");
		fseek(fp, 0, SEEK_END);
		unsigned long len = (unsigned long)ftell(fp);		//find the position of fp pointer in LogedInUser.txt
		if(len==0||len==1)
		{													//if LogedInUser.txt file is empty(i.e, all are logouted),then provides interface to login or register
			printf("1.Login\n2.Register\n\n");
			printf("Enter Your Choice:");
			scanf("%d",&ch);
			switch(ch)
			{
				case 1:
					login();
					break;
				case 2:
					userRegister();
					break;
			}
			fclose(fp);
		}
		else
		{
			fp=fopen("logedInUser.txt","r");
			while(fgets(buff, 255, fp)!=NULL);										//reads the user name from file,,which customer is loged in
			printf("------------------------------------------------------\n");
			printf("\tYou have already Loged In\n");								//if file is empty,i.e, user already loged in,
			printf("------------------------------------------------------\n\n");	//then it directly displays welecome message,instered of login register menu
			loadLogIn(buff);						//again loads the username
			return userAccessMenu(buff);			//function which provides interface to choose options to customes
			fclose(fp);
		}
		
	
	return 0;
}
int selectItem(struct node *head,char* user)
{
	FILE* fp1,*fp2;
	int choice,i,j,ordNum=0,totPrize=0,itemTotCost,quntNo;
	int* order=(int*)malloc(10*sizeof(int)),*qunt=(int*)malloc(10*sizeof(int));
	char ch;
	struct node *temp=head;
	fp1=fopen("orders.txt","a");		//opens file to append the user order data in detail(each items and their cost)
	fp2=fopen("ordersLog.txt","a");		//opens file to append only name and final cost
	fprintf(fp1,"#%s?",user);
	do
	{
		printf("\nEnter Your Order: ");
		scanf("\n%d",&choice);
		printf("Enter Number of Quantities: ");
		scanf("%d",&quntNo);
		printf("\nDo you want to order more[y/n] : ");
		scanf("\n%c",&ch);
		order[ordNum]=choice;
		qunt[ordNum]=quntNo;
		ordNum++;
	}while(ch=='y');
	printf("You have successfully placed order\n\n");
	printf("------------------------------------------------------\n");
	printf("\t\tBILL\n");
	printf("------------------------------------------------------\n");
	fprintf(fp1,"At %s(%s)?",currtRest,location);
	for(j=0;j<ordNum;j++)
	{
		for(i=1;i<order[j];i++)
			temp=temp->rest;
		itemTotCost=(temp->cost)*qunt[j];
		printf("%s - %d X %d = %d/-\n",temp->name,temp->cost,qunt[j],itemTotCost);
		fprintf(fp1,"%s - %d X %d =	%d/-?",temp->name,temp->cost,qunt[j],itemTotCost);		//appends the order details
		totPrize+=itemTotCost;
		temp=head;
	}
	printf("Total Bill Amount : %d/-\n",totPrize);
	fprintf(fp1,"Total Bill Amount : %d/-.\n",totPrize);
	fprintf(fp2,"%s-%d/-\n",user,totPrize);
	fclose(fp1);
	fclose(fp2);
	printf("\nDo you want to go Main menu [y/n] ");			//after placing ordder provide option to go to main menu
	scanf("\n%c",&ch);
	if(ch=='y')
		return userAccessMenu(user);
	return 0;
}
void displayMenu(struct node *head,char* user)			//funtion to display menu in selected restuarent
{
	struct node *temp=head;int i;						//linked list in which each node in LL is single item in Menu
	if(head==NULL)
	printf("yes\n");
	printf("------------------------------------------------------\n");
	printf("\t\tItems In\n");
	printf("------------------------------------------------------\n");
	for(i=1;temp!=NULL;temp=temp->rest)
		printf("%d. %s-%d/-\n",i++,temp->name,temp->cost);
	selectItem(head,user);										//function to select item in menu using LL
	}
struct node* createItemList(struct node* head,char* buff,int i)
{
	struct node* new=(struct node*)malloc(sizeof(struct node));
	if(buff[i]!='\0')
	{
		char itemname[20],cost1[20];
		int k=0;
		for(;buff[i]!='-'&&buff[i]!='\0';i++)		//takes item name from file 
				itemname[k++]=buff[i];
		itemname[k]='\0';
		k=0;
		strcpy(new->name,itemname);
		for(i+=1;buff[i]!='-'&&buff[i]!='\0';i++)	//takes item cost from restuarent file
				cost1[k++]=buff[i];
		cost1[k]='\0';
		new->cost=atoi(cost1);
		new->rest=NULL;
		new->items=NULL;
		if(head==NULL)
			head=new;
		else
		{
			struct node *temp=head;
			for(;temp->rest!=NULL;temp=temp->rest);
				temp->items=new;
	
		}
		head->rest=createItemList(head->rest,buff,i+1);		//funtion which sends LL head and item name,,to create a LL node to append to LL
	}
	else
		return head;
	return head;
}
void displayretaurents(struct node *head)
{
	struct node *temp=head;int i;
	for(i=1;temp!=NULL;temp=temp->rest)				//parsing LL to display restuarent name which are present in a each node of LL
		printf("%d. %s\n",i++,temp->name);
}

void selectRest(struct node *head,char* user)
{
	int choice,i;
	currtRest=(char*)malloc(50*sizeof(char));
	struct node *temp=head;
	printf("Enter Retaurent\n");
	scanf("%d",&choice);
	for(i=1;i<choice;i++)				//based on the select restuarent number the LL will parse and stops ate required restuarent name
		temp=temp->rest;
	strcpy(currtRest,temp->name);
	puts(temp->name);
	puts(currtRest);
	displayMenu(temp->items,user);		//head of that restuarent and username is passed to this funtion to display the items in that seleted restuarent
}
struct node* create(struct node* head,char* buff)
{
	struct node* new=(struct node*)malloc(sizeof(struct node));
	char restname[20];
	int i,k=0;
	
		for(i=0;buff[i]!='-';i++)					//takes restuarent name from file
				restname[k++]=buff[i];				
	restname[k]='\0';								//
	strcpy(new->name,restname);
	new->rest=NULL;
	new->items=NULL;
	new->items=createItemList(new->items,buff,i+1);	//creates LL node to store items under restuarent
	if(head==NULL)
		head=new;
	else
	{
		struct node *temp=head;
		for(;temp->rest!=NULL;temp=temp->rest);
		temp->rest=new;
	
	}
	return head;
}

void restaurentlist(FILE *fp,char* user)
{
struct node *head=NULL;
char buff[500];
while(fgets(buff, 500, fp)!=NULL)
{
	buff[strlen(buff)-1]='\0';
	if(!(strcmp(buff,"end")))	//reads data from buffer untill biffer has a string "end"
		break;
	head=create(head,buff);	
	
}
displayretaurents(head);
selectRest(head,user);
}
void getRestaurentArea(char* location,char* user)
{
	FILE *fp;
	char* buff=(char*)malloc(255*sizeof(char));
	fp=fopen("restaurent.txt","r");
	while(fgets(buff, 255, fp)!=NULL)
	{
		buff[strlen(buff)-1]='\0';			//takes the city name from file
		if(strcmp(buff,location)==0)		//based on provided input city name,it compare set of city name from the restuarent.txt file 
		{
			printf("------------------------------------------------------\n");
			printf("\t\tFood Panda at %s\n",location);
			printf("------------------------------------------------------\n");
			printf("\nThe Restaurents Near To You\n");
			restaurentlist(fp,user);             	                                                                                                           //ewhfbawjhalfjarfvlj
			break;
		}
	}
	fclose(fp);
}

void getLocation(char* user)
{
	location=(char*)malloc(20*sizeof(char));
	int  ch;
	printf("Enter your location:\n\n");
	printf("1.Vizag\n2.Delhi\n3.Hyderabad\n4.Banglore\n5.Chennai\n\n");
		printf("Enter Your Choice:");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				strcpy(location,"Vizag");
				break;
			case 2:
				strcpy(location,"Delhi");
				break;
			case 3:
				strcpy(location,"Hyderabad");
				break;
			case 4:
				strcpy(location,"Banglore");
				break;
			case 5:
				strcpy(location,"Chennai");
				break;
			default:
			printf("Please enter valid ninput\n");
		}
	getRestaurentArea(location,user);		//function to check whether given input city name is found in file or not
}
char * cName;
void findNameByEmail(char* user)
{
		FILE *fp;
		fp=fopen("registers.txt","r");
		char buffer[255];
		cName=(char*)malloc(100*sizeof(char));
		char * email=(char*)malloc(100*sizeof(char));
		int i,j=0;
		while(fgets(buffer, 255, fp)!=NULL)
		{
			for(i=0;buffer[i]!='-';i++)
				email[i]=buffer[i];
			email[i]='\0';
			if(strcmp(email,user)==0)	//checks whether email paramenter is present in file or not
			{
				for(i=i+1;buffer[i]!='-';i++);
				for(i=i+1;buffer[i]!='-';i++,j++)//if email is found then coresponding  name is taken
					cName[j]=buffer[i];
				cName[j]='\0';
			}
		}
		fclose(fp);
}
void loadLogIn(char *user)
{
	FILE *fp;
	fp=fopen("logedInUser.txt","w");
	if(strcmp(user," "))					//if user seleted logOut then whole data in file will be cleared
	{
		findNameByEmail(user);
		printf("Welcome %s !!! \n\n",cName);
		fprintf(fp,"%s",user);
		fclose(fp);
	}
	else							//if user loged in,then his name will be update in file with his email
	{
		fprintf(fp,"%s",user);			
		fclose(fp);
	}
}
int trackOrders(char *user)
{
	FILE *fp;
	int i,j,flag=0;
	char fileUser[100],ch;
	fp=fopen("orders.txt","r");
	char buffer[255];
	while(fgets(buffer, 255, fp)!=NULL)
	{
		i=0;
		if(buffer[i]=='#')							//if buffer starting character is '#',the it means it is user email
		{											//then it checks
			for(i=i+1,j=0;buffer[i]!='?';i++,j++)
				fileUser[j]=buffer[i];
			fileUser[j]='\0';
			if(strcmp(fileUser,user)==0)			//checks whether user email is found in fole or not
			{
				flag=1;
				printf("\n-----------------------------------\n\t");
				for(i=i+1;buffer[i]!='?';i++)
					printf("%c",buffer[i]);
				printf("\n-----------------------------------\n\n");
				for(i=i+1;buffer[i]!='.';i++)
				{
					if(buffer[i]=='?')					// user email is found then content will be displayed
						printf("\n");
					else
						printf("%c",buffer[i]);			
				}	
			}
		}	
	}
	if(flag==0)
			printf("You dont have any logs\n");			//if the user email is not found the it displays the there are no previous orders
	printf("\n\nDo you want to go Main menu [y/n] ");
	scanf("\n%c",&ch);									//provide option to go to customer main menu
	if(ch=='y')
		return userAccessMenu(user);
	return 0;
}
void logOut()
{
	loadLogIn(" ");								//sents empty data,,inorder to clear the data in loged in file
	printf("\nSuccessfully Loged Out\n");			//id customer chooses logOut option then he will be loged out
}
int userAccessMenu(char* user)
{
		int ch;
		printf("1.List of Restuarents\n2.Recent Orders\n3.Logout\n\n");
		printf("Enter Your Choice:");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
					getLocation(user);				//to select the city name
					break;
			case 2:
					return trackOrders(user);		//to display the previous orders history
					//break
			case 3:
					logOut();						//select option to logout
					return -1;
			default:
			printf("Please enter valid input\n");
		}
		return 1;
}
int checkLogin(char *buffer,char *username,char *password)
{
	char* user=(char*)malloc(15*sizeof(char));
	char* pass=(char*)malloc(15*sizeof(char));
	int i,k=0;
	for(i=0;buffer[i]!='-';i++)
		user[i]=buffer[i];
	user[i]='\0';					//takes username form file
	for(i+=1;buffer[i]!='-';i++)
		pass[k++]=buffer[i];
	pass[k]='\0';					//takes password from file
	if(!strcmp(user,username) && !strcmp(pass,password))			// username password validation 
	{
		printf("------------------------------------------------------\n");
		printf("\tSuccessfully Loged In\n");
		printf("------------------------------------------------------\n\n");
		loadLogIn(user);						//after user loged in,then user email will be stored in file
		return userAccessMenu(user);			//after log in,function to display customer menu 
	}
	return 0;
}
void login()
{
	FILE *fp;
	int flag=0;
	fp=fopen("registers.txt","r");
	char* username=(char*)malloc(15*sizeof(char));
	char* password=(char*)malloc(15*sizeof(char));
	printf("Enter the email:");
	scanf("\n");
	gets(username);					//reads username
	printf("Enter the password:");
	gets(password);					//reads password
	char buffer[255];
	while(fgets(buffer, 255, fp)!=NULL)
	{
		if(checkLogin(buffer,username,password))		//function for usename & password validation
		{
				fclose(fp);	
				flag=1;
		}
	}
	if(flag==0)
		printf("\nInvalid credintials\n");
	fclose(fp);	
}
int checkExist(char* email)
{
	FILE* fp;
	fp=fopen("registers.txt","r");
	char buffer[255];
	char* user=(char*)malloc(15*sizeof(char));
	int i;
	while(fgets(buffer, 255, fp)!=NULL)
	{
		for(i=0;buffer[i]!='-';i++)
			user[i]=buffer[i];
		user[i]='\0';				//takes user email from file
		if(!strcmp(user,email))		//if email is already present in file ,,the it retur that already user exists
			return 1;
	}
	return 0;
}
void userRegister()
{
	FILE *fp;
	char* username=(char*)malloc(15*sizeof(char));
	char* password=(char*)malloc(15*sizeof(char));
	char* email=(char*)malloc(40*sizeof(char));
	char* phone=(char*)malloc(13*sizeof(char));
	char* city=(char*)malloc(30*sizeof(char));
	fp=fopen("registers.txt","a");
	printf("Enter Name:");
	scanf("\n");
	gets(username);				//reads username
	printf("Enter password:");
	gets(password);				//reads password
	printf("Enter the valid email address (Which is taken as Username):");
	gets(email);		//reads email
	if(checkExist(email))				//funtion to check whether user alresy exits or not
	{
		printf("This User alredy exists!!! Try again\n");	//if user exists provide option to Re-enter details
		fclose(fp);
		userRegister();
	}
	printf("Enter the phone number:");
	gets(phone);
	printf("Enter the city:");
	gets(city);
	fprintf(fp,"%s-%s-%s-%s-%s\n",email,password,username,phone,city);//upates the custome details in register.txt file
	printf("Successfully Registered\n");
	fclose(fp);
	main();
}
