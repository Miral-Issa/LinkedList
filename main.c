#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct course;
typedef struct course *ptr;

struct student;
typedef struct student *sptr;

struct course {
char name [80];
char id [20];
int year;
int max;
int registered; //keep track of registered students
int waiting_list;//to keep track of students on the waiting list
ptr next;
sptr nextStudent;
};
typedef ptr linked_list;
typedef ptr pos;


struct student {
char student_name [80];
int student_id;
char student_courses [80][20]; // the student is allowed to register for 5 courses only but there's no guarantee that the first 5 courses they apply for will be accepted.
sptr next;
};
typedef sptr LinkedList;


int courses_num=0;
int students_num=0;
void insert_course(linked_list L,pos p, ptr temp);
void printf_list(LinkedList L);
void insert_student(LinkedList L, sptr temp);
void enrollment(linked_list C,LinkedList S,ptr CoursesList []);
void enrolle_student(linked_list C,sptr temp);

void students_report(LinkedList S,int save,char filename [20]);
void courses_report(linked_list C,int save,char filename [20]);
void LessThan5(linked_list C,int save,char filename [20]);
void courseStudents(linked_list C,char id [20],int save,char filename [20]);
void FindStudentInCourse(linked_list C,char cid [20],int sid,int save,char filename [20]);

int main()
{
    linked_list courses; //creating a linked list for the courses
    courses=(linked_list)malloc(sizeof(struct course));
    courses->next=NULL;
    courses->nextStudent=NULL;

    LinkedList students; //creating a linked list for the students
    students=(LinkedList)malloc(sizeof(struct student));
    students->next=NULL;

    //filling the linked lists
    char str [150];
    FILE* fp;
    fp= fopen("courses.txt","r");

    while(fgets(str,100,fp))
    {
        ptr temp;
        temp=(ptr)malloc(sizeof(struct course));

        char *token= strtok(str,",");
        strcpy(temp->name,token);
        token= strtok(NULL,",");
        strcpy(temp->id,token);
        token=strtok(NULL,",");
        temp->year=atoi(token);
        token=strtok(NULL,",");
        temp->max=atoi(token);
        temp->registered=0;
        temp->waiting_list=0;
        temp->nextStudent=NULL;

        insert_course(courses,courses,temp);

    }
    fclose(fp); //no other need for the courses file for its contents are in the linked list now

    fp= fopen("students.txt","r");

    while(fgets(str,100,fp))
    {
        sptr temp;
        temp=(sptr)malloc(sizeof(struct student));

        char *token= strtok(str,"#");
        strcpy(temp->student_name,token);
        token= strtok(NULL,"#");
        temp->student_id=atoi(token);

        token= strtok(NULL,"#");
        int i=0;
        while(token!=NULL)
        {
                strcpy(temp->student_courses[i],token);
                i++;
                token=strtok(NULL,"#");
        }
        token=strtok(temp->student_courses[i-1],"\n"); //for some reason strtok add anew line to the last course in the list maybe because its the last in the sentence in the file.
        //and that creates some errors in the later comparison.
        for(;i<20;i++)
        {
            strcpy(temp->student_courses[i],"+");//that for filling the empty array items with something that i cane compare thus ignore
        }
        insert_student(students,temp);
    }
    fclose(fp); //no other need for the students file

    ptr CoursesList [courses_num]; //equals linked_list courses; that linked_list=ptr

    enrollment(courses,students,CoursesList);


   char id [20];
   int sid=9;


   while(sid!=0)
   {
       printf("**********\n");
    printf("courses and students are already in the system\n");
    printf("1) Print the students' list that contains all students' information\n");
    printf("2) Print a course list that contains, the course name, course id, number of registered students in that course, and number of students in the waiting list for that course\n");
    printf("3) Print a list of all courses that have less than 5 students.\n");
    printf("4) Search for any course based on course ID, if found, print all students ids’ and names’ in this course.\n");
    printf("5) Search for any student based on student ID, in a given course.\n");
    printf("to chose a report inter its number and if you want to Exit inter 0.\n");
   // scanf("%d",&sid);
        int save=0;
        char filename [20]="empty file";

       switch(sid)
       {
       case 0:
            break;
       case 1:
            students_report(students,save,filename);
            printf("do yo want to save the report in a file? if yes inter 1\n");
            scanf("%d",&save);
            if(save ==1)
            {
                printf("inter the file's name\n");
                scanf("%s",filename);
                students_report(students,save,filename);
                printf("file saved\n");
            }
            break;
       case 2:
            courses_report(courses,save,filename);
            printf("do yo want to save the report in a file? if yes inter 1\n");
            scanf("%d",&save);
            if(save ==1)
            {
                printf("inter the file's name\n");
                scanf("%s",filename);
                courses_report(courses,save,filename);
                printf("file saved\n");
            }
            break;
       case 3:
            LessThan5(courses,save,filename);
            printf("do yo want to save the report in a file? if yes inter 1\n");
            scanf("%d",&save);
            if(save ==1)
            {
                printf("inter the file's name\n");
                scanf("%s",filename);
                LessThan5(courses,save,filename);
                printf("file saved\n");
            }
            break;
       case 4:
            printf("please inter the courses id\n");
            scanf("%s",id);
            courseStudents(courses,id,save,filename);
            printf("do yo want to save the report in a file? if yes inter 1\n");
            scanf("%d",&save);
            if(save ==1)
            {
                printf("inter the file's name\n");
                scanf("%s",filename);
                courseStudents(courses,id,save,filename);
                printf("file saved\n");
            }
            break;
       case 5:
            printf("please inter the course's id\n");
            scanf("%s",id);
            printf("please inter the student's id\n");
            scanf("%d",&sid);
            FindStudentInCourse(courses,id,sid,save,filename);
            printf("do yo want to save the report in a file? if yes inter 1\n");
            scanf("%d",&save);
            if(save ==1)
            {
                printf("inter the file's name\n");
                scanf("%s",filename);
                FindStudentInCourse(courses,id,sid,save,filename);
                printf("file saved\n");
            }
            break;
       default:
            printf("command not found\n");
       }
       printf("**********\n");
       scanf("%d",&sid);
   }


    return 0;
}

void insert_course(linked_list L,pos p, ptr temp)//insert the course's data in the linked list of courses
{
    if(temp!= NULL && L!=NULL)
    {
        while(p->next!=NULL && (strcmp(temp->name,p->next->name) > 0))
        {
            p=p->next;
        }

        temp->next=p->next;
        p->next=temp;
        //printf("course added\n");
        courses_num++;
    } else
        printf("error in insertion\n");
}
void insert_student(LinkedList L, sptr temp)//insert the student's data in the linked list of students
{
    sptr p=L;
    if(temp!= NULL && L!=NULL)
    {
        while(p->next!=NULL && (temp->student_id >= p->next->student_id))//ordered based on student’s ID
        {
            p=p->next;
        }

        temp->next=p->next;
        p->next=temp;
        //printf("student added\n");
        students_num++;
    } else
        printf("error in insertion\n");
}
void enrollment(linked_list C,LinkedList S,ptr CoursesList [])//the function list the course's enrolled students attached to it
{
    //ptr CoursesList [courses_num]; //equals linked_list courses; that linked_list=ptr
    ptr cp,lp;
    cp=C->next;
    for(int i=0;i<courses_num;i++)//i created an array of pointers each point to a course's node and with that it creates a sense of array linked list
    {
        CoursesList[i]=(linked_list)malloc(sizeof(struct course));
        CoursesList[i]->next=cp;
        lp=CoursesList[i]->next;
        cp=cp->next;
    }

    sptr sp;
    sp=S->next;
    for(int i=0;i<students_num;i++)//to check every student
    {
       int reach5=0;
       int j=0;
       while(strcmp(sp->student_courses[j],"+")!=0)//to check every course the student signs for but not the empty array items
       {
           int found=0;

          for(int k=0;k<courses_num;k++)//to check every course that the system have in the linked list
          {
            lp=CoursesList[k]->next;

            if(strcmp(sp->student_courses[j],lp->id)==0)
            {
                found=1;
                if(reach5 >= 5)
                {
                    printf("the student %s %d registered 5 courses and can't register in more\n",sp->student_name,sp->student_id);
                }else
                {
                    int year = lp->year*10000 +9999;
                    //printf("%d\n",year);
                    if(sp->student_id > year)
                    {
                        printf("the student %s %d isn't allowed to register for this course %s because it's for older students who registered in previous years\n",sp->student_name,sp->student_id,lp->id);
                    }else
                    {
                    //creating a duplicate for the student data
                    sptr temp;
                    temp=(sptr)malloc(sizeof(struct student));
                    strcpy(temp->student_name,sp->student_name);
                    temp->student_id=sp->student_id;
                    for(int h=0;h<20;h++)
                    {
                        strcpy(temp->student_courses[h],sp->student_courses[h]);
                    }

                    enrolle_student(lp,temp);//similar to insert_student but it adds the student to the course

                    if(lp->registered>=lp->max)//if the max number of students is reached then the student is added to the waiting list counter
                    {
                        lp->waiting_list+=1;
                        printf("The course is closed and the student %s %d has been added to the waiting list\n",sp->student_name,sp->student_id);
                    }else
                        lp->registered+=1;

                    reach5++;


                    }//if else year
                }//reach5 if else

            }//if found


            }
            if(found==0)
                printf("course %s not found\n",sp->student_courses[j]);


           j++;
       }

        sp=sp->next;
    }
}
void enrolle_student(linked_list C,sptr temp)
{
    if(temp!= NULL && C!=NULL)
    {

        temp->next=C->nextStudent;
        C->nextStudent=temp;
        //printf("student added\n");
    } else
        printf("error in insertion\n");
}
void students_report(LinkedList S,int save,char filename [20])
{
    sptr p=S->next;
    FILE *fp;
    fp=fopen(filename,"w");
    while(p!=NULL)
    {
        if(save ==1)
        {
            fprintf(fp,"\n %s\t %d\t",p->student_name,p->student_id);
        }else
            printf("%s\t %d\t",p->student_name,p->student_id);

        int i=0;
        while(strcmp(p->student_courses[i],"+")!=0)
        {
            if(save == 1)
            {
                fprintf(fp,"%s\t",p->student_courses[i]);
            }else
                printf("%s\t",p->student_courses[i]);

            i++;
        }
        if(save == 0)
            printf("\n");

        p=p->next;
    }
}
void courses_report(linked_list C,int save,char filename [20])
{
    ptr p=C->next;
    FILE *fp;
    fp=fopen(filename,"w");
    while(p!=NULL)
    {
        if(save == 1)
            fprintf(fp,"%s\t %s\t %d\t %d\n",p->name,p->id,p->registered,p->waiting_list);
        else
            printf("%s\t %s\t %d\t %d\n",p->name,p->id,p->registered,p->waiting_list);
        p=p->next;
    }

}
void LessThan5(linked_list C,int save,char filename [20])
{
    ptr p=C->next;
    FILE *fp;
    fp=fopen(filename,"w");

    while(p!=NULL)
    {
        if(p->registered < 5)
            if(save == 1)
                fprintf(fp,"%s\t %s\n",p->name,p->id);
            else
                printf("%s\t %s\n",p->name,p->id);

        p=p->next;
    }
}
void courseStudents(linked_list C,char id [20],int save, char filename [20])
{
    ptr p=C->next;
    int found =0;
    FILE *fp;
    fp=fopen(filename,"w");

    while(p!=NULL)
    {
        if(strcmp(p->id,id)==0)
        {
            found=1;
            sptr sp=p->nextStudent;
            for(int j=0;j<p->waiting_list;j++)
            {
                sp=sp->next;
            }
            for(int i=0;i<p->registered;i++)
            {
                if(save == 1)
                    fprintf(fp,"%d\t %s\n",sp->student_id,sp->student_name);
                else
                    printf("%d\t %s\n",sp->student_id,sp->student_name);

                sp=sp->next;
            }
        }
        p=p->next;
    }
    if(found==0)
        printf("not found\n");
}
void FindStudentInCourse(linked_list C,char cid [20],int sid,int save, char filename [20])
{
    ptr p=C->next;
    int foundc=0;
    int founds=0;
    FILE *fp;
    fp=fopen(filename,"w");

    while(p!=NULL)
    {
        if(strcmp(p->id,cid)==0)
        {
            foundc=1;
            sptr sp=p->nextStudent;
            while(sp!=NULL)
            {
                if(sp->student_id==sid)
                {
                    founds=1;
                    if(save == 1)
                        fprintf(fp,"%s\n",sp->student_name);
                    else
                        printf("%s\n",sp->student_name);
                }
                sp=sp->next;
            }
            if(founds==0)
                printf("student not found\n");
        }
        p=p->next;
    }
    if(foundc==0)
        printf("course not found\n");
}
