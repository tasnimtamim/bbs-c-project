//A fully working BBS/User forum/Social media written entirely on C language and terminal interface
//Lab Project by Tasnim Hasan Tamim - ID:251 256 86 42 - CSE115.11

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<string.h>

//Colors for text
#define CYAN    "\x1B[36m"
#define MAGENTA "\x1B[35m"
#define RESET   "\x1B[0m"
#define YELLOW  "\x1B[33m"
#define GREEN   "\x1B[32m"
#define RED     "\x1B[31m"

//Global Variable - For storing logged in username to use everywhere
char loggedInUser[30] = "";

// Function Prototypes - causing bug so im using prototype which fixes it
void make_post(const char *category_filename);
void view_post(const char *category_filename, int post_number);
int list_posts(const char *filename);

//FUNCTION ONLY TO TELL CURRENT TIME
void printCurrentTime() {
    time_t t; //time_t is a datatype from time.h to store time data. we take a variable t as time_t datatype
    struct tm *tm_info; //we create a structure tm. tm can hold time separated by hour, min and second. then we point it to pointer named tm_info
    //tm_info = is a structure which holds time structure. this will later store our time

    time(&t); //our t variable then gets current time from computer system
    tm_info = localtime(&t); //localtime turns raw time data converts to regional time. example UTC time converts to Bangladeshi UTC+6.
    //our tm_info structure is filled in with this.

    printf("\t\tCurrent server time: %s", asctime(tm_info));
}

//WELCOME HOMEPAGE
void welcome(int login_status){
    printf(CYAN);
    printf("\n\t================================================================\n");
    printf("\t\tWelcome to NSU Bulletin Board Service\n");
    printCurrentTime();
    if(login_status==1){ //self explanatory
        printf("\t\tCurrently logged in user = %s.\n",loggedInUser);
    }
    else{printf("\t\tCurrently logged in user = Not logged in.\n");}
    printf("\t================================================================\n");
    printf("\n\n");
    printf("Please select an option from the following:\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Enter Forum\n");
    printf("4. Exit\n");
    printf(RESET);
    printf("Enter your choice: ");
}

//AUTHENTICATE GENUINE USER. BASICALLY READS FROM TEXT FILE USERNAME AND PASSWORD
int login(){
    printf(MAGENTA);
    printf("\n\n");
    printf("\n\t================================================================\n");
    printf("\t\tLogging In - Welcome Back!\n");
    printf("\t================================================================\n");
    printf("\n\n");

    char username[30];
    char password[30];
    char f_username[30];
    char f_password[30];

    printf("Please enter your username:");
    scanf("%s",username);

    printf("Please enter your password:");
    scanf("%s",password);

    int isAuthenticated = 0; //assume user isnt authenticated in first place

    FILE *registration = fopen("register.txt","r"); //We reading a register.txt file in READ mode
    if (registration == NULL) { //checking if register.txt file exists in the first place. else code will give error.
        printf(RED "[\u2716] Error: Could not open file for registration.\n" RESET);
        return 0;
    }

    while(fscanf(registration, "%s %s",f_username,f_password) == 2){
        if(strcmp(username, f_username) == 0 && strcmp(password,f_password) == 0){
            isAuthenticated = 1;
            strcpy(loggedInUser,username);
            break;
        }
    }

    fclose(registration);

    if(isAuthenticated == 1){
        printf("[\u2714] You have been authenticated successfully to enter BBS. Welcome member, %s.\n\n",username);
    }
    else{
        printf(RED "[\u2716] Invalid User credential. Please try again\n\n" RESET);
    }
    printf(RESET);
    return isAuthenticated;
}

//REGISTERS NEW USER. BASICALLY WRITES USERNAME AND PASSWORD TO A TEXTFILE
void registering(){
    printf(GREEN);
    printf("\n\n");
    printf("\n\t================================================================\n");
    printf("\t\tRegistering User - Welcome to NSU BBS\n");
    printf("\t================================================================\n");
    printf("\n\n");

    FILE *registration = fopen("register.txt","a");

    char username[30];
    char password[30];

    printf("\nRequirement: Username and Password cannot exceed 30 character.\n");
    printf("Please enter your username: ");
    fgets(username,30,stdin);
    username[strcspn(username, "\n")] = 0;

    printf("Please enter your password: ");
    fgets(password,30,stdin);
    password[strcspn(password, "\n")] = 0;

    fprintf(registration,"%s\t%s\n",username,password);

    fclose(registration);
    printf("[\u2714]You have been successfully registed as %s. Please login now to be authenticated to access BBS.\n\n",username);
    printf(RESET);
    return;
}

//HANDLES WELCOME/HOMEPAGE CHOICES for registering,logging in,entering forum or exiting program
int handle_choice(int choice, int login_status){
    switch(choice){
        case 1:
            login_status = login();
            break;
        case 2:
            registering();
            break;
        case 4:
            break;
        default:
            printf(RED "[\u2716] Invalid choice\n" RESET);
            break;
    }
    return login_status;
}

//FUNCTIONS TO HANDLE MESSAGING SYSTEM BETWEEN USERS. inbox() connects functionality of send_message() and read_message()
void send_message(){
    char to[30]; //holds name of person im messaging
    char chatTextFile[30]; //holds text format
    char messages[200]; //holds message

    printf("\nWho you want to send message to? (Enter exact username of user, case-sensetive):");
    while ((getchar()) != '\n'); //IMPORTANT found in debugging - Used to clear input buffer. Else receiver name gets SKIPPED. Crutial step
    fgets(to,30,stdin); //reading receiever of my message
    to[strcspn(to, "\n")] = 0; // remove newline - DEBUGGING HELL

    printf("\nPlease enter your message below:\n");
    fgets(messages,200,stdin); //Stores the message 200 word limit
    messages[strcspn(messages, "\n")] = 0; // Remove newline - AGAINNNNNNNNN

    //I use string compare so person A and person B names will be stored in text file format in alphabetical order
    //so A = Tamim, B = Imran
    //string compare, since Tamim (T) is greater than Imran (I), the filename will ALWAYS be chatlog_Tamim_Imran.txt
    //Imran messages Tamim, filename = chatlog_Tamim_Imran.txt, Tamim messages Imran, filename = chatlog_Tamim_Imran
    //Tamim will ALWAYS have priority over Imran
    //TL;DR -> This ensures SAME FILE NAME regardless of user.

    if (strcmp(loggedInUser, to) < 0) {
        sprintf(chatTextFile, "chatlog_%s_%s.txt", loggedInUser, to); //Formatting for text file containing chat log. ex: chatlog_tamim_imran.txt
    } else {
        sprintf(chatTextFile, "chatlog_%s_%s.txt", to, loggedInUser); //Formatting for text file containing chat log
    }

    FILE *messaging = fopen(chatTextFile,"a"); //creates file with append propertise

    if (messaging == NULL) {
        printf("Error: Could not open message file.\n");
        return;
    }
    //fprintf(registration,"%s\t%s\n",username,password);
    fprintf(messaging,"%s:%s\n",loggedInUser,messages);
    fclose(messaging);

}
void read_message(){
    char from[30]; //who they want to read messages with
    char chatTextFile[30]; //holds text format
    char message[200]; //holds message

    printf("\nWith whom you want to check message logs with? (Enter exact username of user, case-sensetive):");
    while ((getchar()) != '\n'); //AGAIN, IMPORTANT INPUT BUFFER fix
    fgets(from,30,stdin); //reading receiever of my message
    from[strcspn(from, "\n")] = 0; // remove newline

    //now lets generate file name TO SEE IF A TEXT FILE EXISTS WITH SAME FILENAME. Copypaste code from send_message()
    if (strcmp(loggedInUser, from) < 0) {
        sprintf(chatTextFile, "chatlog_%s_%s.txt", loggedInUser, from); //Formatting for text file containing chat log. ex: chatlog_tamim_imran.txt
    } else {
        sprintf(chatTextFile, "chatlog_%s_%s.txt", from, loggedInUser); //Formatting for text file containing chat log
    }

    // Open the chat file in read mode
    FILE *messaging = fopen(chatTextFile, "r");
    //checks if file exists at all
    if (messaging == NULL) {
        printf("\nNo messages found with %s.\n", from);
        return;
     }

     printf("\nChat History with %s:\n", from);
     printf("=====================================\n");

    //read each line from the file and print it
    //logic: fgets reads only 200 character of each line message stored in chatlog.
    //each 200 character reading finished = 1st loop finished
    //and the loop continues until fgets give NULL thats when it stops
    //learning: fgets as long it's reading valid conditions WILL ALWAYS RETURN TRUE VALUE. but as soon as it's invalid it'll give NULL. I'm reading only 200 lines each every loop.
    //after reading a line fgets() adds a null terminator ('\0') at the end of the string (effectively marking the end of the line).
    //that's how while loop goes down the text file line.
    while (fgets(message,200,messaging)) {
        printf("%s", message);
    }

    printf("=====================================\n");

    fclose(messaging);
}
void inbox(){
    printf("\n\t================================================================\n");
    printf("\t\t[\u2605 ] Welcome %s",loggedInUser); printf("\t[\u2709 ] Inbox Center");
    printf("\n\t================================================================\n");
    printf("\n1. Send Message\n2.Read Message\nPlease enter your choice:");
    int choice;
    scanf("%d",&choice);
    switch(choice){
        case 1:
            send_message();
            break;
        case 2:
            read_message();
            break;
        default:
            printf("Invalid Choice");
            break;
    }
}

//function to call when user MAKES a post
void make_post(const char *category_filename) {
    //making it const makes it so value is CONSTANT, that is, protect the value being pointed to. this function will not modify the string being pointed to
    char title[100], body[1000];
    //this part is used to timestamp each of users post
    time_t now;
    time(&now); //we fill now variable with timestamp

    FILE *fp = fopen(category_filename, "a");
    if (!fp) {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    //writing post title
    printf(YELLOW "\nEnter post title: ");
    while ((getchar()) != '\n'); // clear input buffer before taking fgets
    fgets(title,100, stdin);
    title[strcspn(title, "\n")] = 0; //removing extra newline/input overflow fix

    //writing post body paragraph
    printf("Enter post body:\n");
    fgets(body,1000, stdin);
    body[strcspn(body, "\n")] = 0;  //newline remove same thing

    char *timestamp = ctime(&now); //these extra lines are used BECAUSE it was causing issues reading post. under normal circumstances fprintf(fp, "%s||%s||%s||%s\n", loggedInUser, ctime(&now), title, body); would suffice
    timestamp[strcspn(timestamp, "\n")] = 0; // Remove newline
    fprintf(fp, "%s||%s||%s||%s\n", loggedInUser, timestamp, title, body); //structure of the post's title. ctime to convert time to readable variable like sun,apr,20,time 2025
    fclose(fp);
    printf(GREEN "\n[\u2714] Post added successfully!\n" RESET);
}

//function to call when user wants to READ a post
void view_post(const char *category_filename, int post_number) {
    FILE *fp = fopen(category_filename, "r");
    if (!fp) {
        printf(RED "Couldn't open file.\n" RESET);
        return;
    }

    char line[1200];
    int current = 1;
    while (fgets(line, sizeof(line), fp)) {
        if (current == post_number) {
            char *author = strtok(line, "||");
            char *time = strtok(NULL, "||");
            char *title = strtok(NULL, "||");
            char *body = strtok(NULL, "||");

            printf(GREEN "\nPost #%d\n-----------\nAuthor: %s\nTime: %sTitle: %s\n\n%s\n" RESET,
                   post_number, author, time, title, body);
            fclose(fp);
            return;
        }
        current++;
    }
    printf(RED "Post not found.\n" RESET);
    fclose(fp);
}

int list_posts(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf(RED "Couldn't open file.\n" RESET);
        return 0;
    }

    char line[1200];
    int post_num = 1;

    printf("\nAvailable Posts:\n");
    printf("=========================\n");

    while (fgets(line, sizeof(line), fp)) {
        char *author = strtok(line, "||");
        strtok(NULL, "||"); // Skip time
        char *title = strtok(NULL, "||");

        if (author && title) {
            printf("%d. %s (by %s)\n", post_num++, title, author);
        }
    }

    printf("=========================\n");

    fclose(fp);
    return post_num - 1;
}

// Shared category handler
void category_menu(const char *category_name, const char *filename) {
    int choice;
    printf("\n\t================================================================\n");
    printf("\t\t[\u2605 ] Welcome %s\t[\u2709 ] %s Section", loggedInUser, category_name);
    printf("\n\t================================================================\n");
    printf("\n1. Make a post\n2. View posts\nEnter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        make_post(filename);
    } else if (choice == 2) {
        int total = list_posts(filename);
        if (total == 0) return;

        int sel;
        printf("\nEnter post number to read full post: ");
        scanf("%d", &sel);
        if (sel > 0 && sel <= total) {
            view_post(filename, sel);
        } else {
            printf(RED "Invalid post number.\n" RESET);
        }
    } else {
        printf(RED "Invalid choice.\n" RESET);
    }
}


//CATEGORIES/SUBJECTS to post about in forum
void news()        { category_menu("News", "posts_news.txt"); }
void sports()      { category_menu("Sports", "posts_sports.txt"); }
void entertainment(){ category_menu("Entertainment", "posts_entertainment.txt"); }
void memes()       { category_menu("Memes", "posts_memes.txt"); }
void technsci()    { category_menu("Technology and Science", "posts_technsci.txt"); }

//FORUM HOMEPAGE/Menu
void bbs_menu(int login_status){
        int bbs_menu_choice;
        printf("\n\n");
        printf("\n\t================================================================\n");
        printf("\t\t[ \u2605 ] Welcome %s",loggedInUser);
        printf("\n\t================================================================\n");
        printf("\nPlease select any categories below:");
        printf("\n1.Inbox\n2.News\n3.Sports\n4.Entertainment\n5.Memes\n6.Technology and Science\n");
        printf("\nYour choice:");
        scanf("%d",&bbs_menu_choice);

        switch(bbs_menu_choice){
            case 1:
                inbox();
                break;
            case 2:
                news();
                break;
            case 3:
                sports();
                break;
            case 4:
                entertainment();
                break;
            case 5:
                memes();
                break;
            case 6:
                technsci();
                break;
            default:
                printf("Invalid option");
                break;
        }
}

//Main function
int main(){
    int choice;
    int login_status = 0;
    while(1){
        welcome(login_status);

        scanf("%d", &choice);
        getchar();

        if(choice==4){
            printf(RED "\nGoodbye!\n" RESET);
            break;}
        if(choice == 3){
            if(login_status==1){
            bbs_menu(login_status);
            }
            else{
                printf(RED "[\u2716] You must be logged in to access this forum." RESET);
            }
        }
        else {
            login_status = handle_choice(choice, login_status);
            }
    }
    return 0;
}
