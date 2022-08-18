/*

Author: Dustin Fraze (df@cromulence.co)

Copyright (c) 2014 Cromulence LLC

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include <libcgc.h>
#include <stdlib_cgc.h>
#include <stdio.h>
#define YOLO_ADD 0x40
#define YOLO_DEL 0x41
#define YOLO_EDIT 0x42
#define YOLO_SHOW 0x43
#define YOLO_LIST 0x44
#define YOLO_SORT 0x45
#define YOLO_EXIT 0x46

unsigned int cookie = 0;

typedef struct _contact {
	char first[32];
	char last[32];
	char phone[16];
	unsigned short officenum;
	char gender;
	char hacker;
	unsigned int cookie;
	int pad;
	struct _contact *prev;
	struct _contact *next;
} contact;

contact *head = NULL;

void put(char *str) {
	//transmit(1, str, strlen_cgc(str), 0);
}

int validateName(char *name)
{
	//let's make it title case.  And lets yell if its not.
	if(name[0] >= 'A' && name[0] <= 'Z')
		return 1;
	else
		return 0;
}

int validatePhone(char *phonenumber)
{
	int i;
	char *digit;

	digit = phonenumber;
	if(*digit++ != '(')
		return 0;
	for(i=0;i<3;i++,digit++)
	{
		if(*digit < '0' || *digit > '9')
			return 0;
	}
	if(*digit++ != ')')
		return 0;
	for(i=0;i<3;i++,digit++)
	{
		if(*digit < '0' || *digit > '9')
			return 0;
	}
	if(*digit++ != '-')
		return 0;
	for(i=0;i<4;i++,digit++)
		if(*digit < '0' || *digit > '9')
			return 0;
	return 1;
}

contact *createContact(char *first, char *last, char *phone, unsigned short officenum, char gender, char hacker)
{
	contact *ret = NULL;
	size_t_cgc strlen_int = 0;
	ret = (contact *)malloc_cgc(1 * sizeof(contact));
	if(ret == NULL)
	{
		//puts_cgc("Couldn't allocate contact.  Something's wrong.");
		return NULL;
	}
	if(validateName(first)) {
		strlen_int = strlen_cgc(first);
		memcpy_cgc(ret->first, first, strlen_int);
		ret->first[strlen_int] = '\0';
	} else {
		//puts_cgc("Invalid first name.  Must begin with a capital letter.");
		free_cgc(ret);
		ret = NULL;
		return NULL;
	}
	if(validateName(last)) {
		strlen_int = strlen_cgc(last);
		memcpy_cgc(ret->last, last, strlen_int);
		ret->last[strlen_int] = '\0';
	} else {
		//puts_cgc("Invalid last name.  Must begin with a capital letter.");
		free_cgc(ret);
		ret = NULL;
		return NULL;
	}
	if(validatePhone(phone)) {
		strlen_int = strlen_cgc(phone);
		memcpy_cgc(ret->phone, phone, strlen_cgc(phone));
		ret->phone[strlen_int] = '\0';
	} else {
		//puts_cgc("Invalid phone number...must look like (nnn)nnn-nnnn");
		free_cgc(ret);
		ret = NULL;
		return NULL;
	}
	ret->officenum = officenum;
	ret->gender = gender;
	ret->hacker = hacker;
	ret->cookie = cookie;
	ret->pad = 0;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}

void insert(contact *toAdd)
{
	contact *tmp;
	if(head == NULL) {
		head = toAdd;
		return;
	}
	tmp = head;
	while(tmp->next != NULL) {
		tmp = tmp->next;
	}
	tmp->next = toAdd;
	toAdd->prev = tmp;
}

contact *remove_cgc(contact *toRemove)
{
	if((toRemove->next == NULL) && (toRemove->prev == NULL)) {
		head = NULL;
		return toRemove;
	}

	if(toRemove->prev == NULL) {
		head = toRemove->next;
		toRemove->next->prev = NULL;
	}
	else
		toRemove->prev->next = toRemove->next;

	if(toRemove->next == NULL)
		toRemove->prev->next = NULL;
	else
		toRemove->next->prev = toRemove->prev;

	toRemove->next = NULL;
	toRemove->prev = NULL;
	return toRemove;
}

void printGender(char gender)
{
	switch(gender)
	{
		case 'M':
			puts_cgc("Male");
			break;
		case 'F':
			puts_cgc("Female");
			break;
		case 'T':
			puts_cgc("Trans-Gender");
			break;
		default:
			puts_cgc("Other");
	}
}

void printContact(contact *toPrint)
{
	//put("First Name: \t");
	//puts_cgc(toPrint->first);
	//put("Last Name: \t");
	//puts_cgc(toPrint->last);
	//put("Phone Number: \t");
	//puts_cgc(toPrint->phone);
}

void printList() {
	contact *ptr;
	ptr = head;
	if(head == NULL){
		//puts_cgc("Empty yolodex.");
	}
		
	while(ptr != NULL)
	{
		//puts_cgc("========================================");
		printContact(ptr);
		ptr = ptr->next;
	}
	//puts_cgc("========================================");
}

void sortYolodex() {
	contact *ptr;
	contact *tmp;
	contact *first;
	contact *other;
	contact *sorted = NULL;
	int i;

	ptr = head;
	first = head;
	tmp = head;

	while(tmp != NULL)
	{
		while(ptr != NULL)
		{
			i = 0;
			while(first->last[i] && ptr->last[i])
			{
				if(first->last[i] > ptr->last[i]) {
					first = ptr;
					break;
				}
				if(first->last[i] == ptr->last[i])
					i++;
				else
					break;
			}
			if(!ptr->last[i] && first->last[i])
				first = ptr;
			ptr = ptr->next;
		}
		tmp = remove_cgc(first);
		if(sorted == NULL)
		{
			sorted = first;
		} else {
			other = sorted;
			while(other->next != NULL)
				other = other->next;
			other->next = tmp;
			tmp->prev = other;
		}
	first = head;
	ptr = head;
	tmp = head;
	}
	head = sorted;
}

void initYolodex() {
	head = createContact("Robert", "Morris", "(617)253-5982", 32972, 'M', 'y');
	insert(createContact("Chareth", "Cutestory", "(123)456-7890", 4242, 'M', 'n'));
	insert(createContact("Tim", "Berners-Lee", "(638)844-2074", 404, 'M','y'));
	insert(createContact("Pwnies", "McPwn", "(222)223-1337", 31337, 'F','y'));
	insert(createContact("Jenny", "Fakename", "(555)867-5309", 31337, 'F','n'));
}

int recvUntil(char *dst, int n, char delim)
{
	char *cur;
	size_t_cgc gotBytes;
	cur = dst;
	while(n--)
	{
		if (receive_cgc( STDIN, cur, 1, &gotBytes) != 0){
			return -1;
		}
		if(gotBytes == 0) {
			return -1;
		}
		if(*cur == delim) {
			*cur = 0;
			break;
		}
		cur++;
	}
	//printf("recevUntil read: %d\n", cur - dst);
	return cur-dst;
}

int promptAdd() {
	char first[32];
	char last[32];
	char phone[16];
	char officetmp[8];
	short officenum;
	char gender;
	char hacker;
	char tmp[2];

	contact *ptr;
	memset_cgc(phone, 0, 16);
	memset_cgc(officetmp, 0, 8);
	ptr = NULL;
	while(ptr == NULL) {
		put("First: ");
		if (recvUntil(first, 32, '\n') == -1){
			return -1;
		}
		put("Last: ");
		if(recvUntil(last, 32, '\n') == -1){
			return -1;
		}
		put("Phone Number: ");
		if(recvUntil(phone, 14, '\n') == -1){
			return -1;
		}
		put("Office Number: ");
		memset_cgc(officetmp, 0, 8);
		if(recvUntil(officetmp, 6, '\n') == -1){
			return -1;
		}
		officenum = (unsigned short)atoi(officetmp);
		put("Gender: ");
		if(recvUntil(tmp, 2, '\n') == -1){
			return -1;
		}
		gender = tmp[0];
		hacker = '?';
		while(hacker == '?')
		{
			put("Hacker: ");
			if(recvUntil(tmp, 2, '\n') == -1){
				return -1;
			}
			if(tmp[0] == 'n' || tmp[0] == 'y')
				hacker = tmp[0];
			else
				puts_cgc("Please enter y or n for hacker.");
		}
		ptr = createContact(first, last, phone, officenum, gender, hacker);
	}
	if(ptr->cookie != cookie)
	{
		puts_cgc("Contact corruption detected.");
		return -1;
	}
	insert(ptr);
	return 0;
}

contact *findContact() {
	char first[32];
	char last[32];
	contact *ptr;
	contact *ret = NULL;
	put("First: ");
	if(recvUntil(first, 32, '\n') == -1){
		return NULL;
	}
	put("Last: ");
	if(recvUntil(last, 32, '\n') == -1){
		return NULL;
	}

	ptr = head;
	while(ptr != NULL) {
		//printf("first:%s, ptr_first:%s\n",first, ptr->first);
		if(strcmp_cgc(first, ptr->first) == 0) {
			//printf("last:%s, ptr_last:%s\n",last, ptr->last);
			if(strcmp_cgc(last, ptr->last) == 0) {
				ret = ptr;
				break;
			}
		}
		ptr = ptr->next;
	}
	//printf("findContact:%x\n", (unsigned int)ret);
	return ret;
}

void promptDel() {
	contact *ptr;

	ptr = findContact();
	if(ptr != NULL) {
		ptr = remove_cgc(ptr);
		free_cgc(ptr);
		puts_cgc("Contact removed.");
	} else {
		puts_cgc("No such contact found.");
	}
}

int editUser(contact *ptr) {
	char data[32];
	char number[8];

	if(ptr != NULL) {
		puts_cgc("Updating fields.  Send just a newline to keep old data.");
		put("New first name: ");
		if(recvUntil(data, 32, '\n') == -1){
			return -1;
		}
		if(data[0] && validateName(data) == 0){
			puts_cgc("Not a legal first name.  Stopping edit.");
			return 0;
		}
#ifdef PATCHED
		data[31] = 0;
#endif
		if(data[0] != 0) {
			memset_cgc(ptr->first, 0, 32);
			memcpy_cgc(ptr->first, data, strlen_cgc(data));
		}
		put("New last name: ");
		if(recvUntil(data, 32, '\n') == -1){
			return -1;
		}
#ifdef PATCHED
		data[31] = 0;
#endif

		if(data[0] && validateName(data) == 0) {
			puts_cgc("Not a legal last name.  Stopping edit.");
			return 0;
		}
		if(data[0] != 0){
			memset_cgc(ptr->last, 0, 32);
			memcpy_cgc(ptr->last, data, strlen_cgc(data));
		}
		put("New phone number: ");
		if(recvUntil(data, 14, '\n') == -1){
			return -1;
		}
#ifdef PATCHED
		data[13] = 0;
#endif
		if(data[0] && validatePhone(data) == 0) {
			puts_cgc("Not a legal phone number.  Stopping edit.");
			return 0;
		}
		if(data[0] != 0) {
			memset_cgc(ptr->phone, 0, 16);
			memcpy_cgc(ptr->phone, data, strlen_cgc(data));
		}
		put("New office number: ");
		if(recvUntil(data, 6, '\n') == -1){
			return -1;
		}
		if(data[0]) {
			memcpy_cgc(number, data, 5);
			ptr->officenum = atoi(number);
		}
		put("New gender: ");
		if(recvUntil(data, 2, '\n') == -1){
			return -1;
		}
		if(data[0])
			ptr->gender = data[0];
		put("Is the user a hacker? ");
		if(recvUntil(data, 2, '\n') == -1){
			return  -1;
		}
		if(data[0])
			ptr->hacker = data[0];
	}
	if(ptr->cookie != cookie)
	{
		puts_cgc("Contact corruption detected.");
		return -1;
	}
	return 0;
}

int promptEdit() {
	contact *ptr;
	//char data[32];
	char number[8];

	memset_cgc(number, 0, 8);
	ptr = findContact();
	if(ptr == NULL) {
		puts_cgc("No such contact.");
		return 0;
	}
	if (editUser(ptr) == -1){
		return -1;
	}
	return 0;
}

int promptShow() {
	contact *ptr;
	char office[8];
	char useraction[2];

	ptr = findContact();
	if(ptr == NULL) {
		puts_cgc("No such contact.");
		return 0;
	}
	memset_cgc(useraction, 0, 2);
	useraction[0] = 's';
	while(useraction[0] != 'q') {
		memset_cgc(office, 0, 6);
		puts_cgc("****************************************");
		put("First name:\t");
		puts_cgc(ptr->first);
		put("Last name:\t");
		puts_cgc(ptr->last);
		put("Phone num:\t");
		puts_cgc(ptr->phone);
		itoa_cgc(office, ptr->officenum, 6);
		put("Office Number:\t");
		puts_cgc(office);
		put("Gender:\t\t");
		printGender(ptr->gender);
		if(ptr->hacker == 'y')
		{
			puts_cgc("[\x1B[31m!\x1B[0m]This user known to be a hacker[\x1B[31m!\x1B[0m]");
		}
		puts_cgc("****************************************");
		if(recvUntil(useraction, 2, '\n') == -1){
			return -1;
		}
		switch(useraction[0]) {
			case 'q':
				break;
			case 'd':
				if(ptr->next == NULL && ptr->prev == NULL) {
					remove_cgc(ptr);
					puts_cgc("No more contacts.  Leaving detail view.");
					useraction[0]='q';
				} else if (ptr->next == NULL) {
					ptr = ptr->prev;
					remove_cgc(ptr->next);
				} else {
					ptr = ptr->next;
					remove_cgc(ptr->prev);
				}
				break;
			case 'e':
				if(editUser(ptr) == -1){
					return -1;
				}
				break;
			case 'n':
				if(ptr->next != NULL)
					ptr = ptr->next;
				else
					puts_cgc("No next contact.");
				break;
			case 'p':
				if(ptr->prev != NULL)
					ptr = ptr->prev;
				else
					puts_cgc("No previous contact.");
		}
	}
	return 0;
}

int initCookies() {
	size_t_cgc bytes;
	//int got = 0;
	//int shift = 0;

	//contact *ptr;

	random_cgc(&cookie, 4, &bytes);
	if(bytes != 4)
	{
		puts_cgc("Couldn't get 4 random bytes.  Panic!");
		return -1;
	}
	cookie |= 0x10204080;
	return 0;
}

int inputLoop() {
	char action[2];
	while(1)
	{
		memset_cgc(action, 0, 2);
		if(recvUntil(action, 2, '\n') == -1){
			return -1;
		}
		//printf("action:0x%x,0x%x\n",action[0],action[1]);
		switch(action[0])
		{
			case YOLO_ADD:
				//printf("YOLO_ADD\n");
				if( promptAdd() == -1){
					return -1;
				}
				break;
			case YOLO_DEL:
				//printf("YOLO_DEL\n");
				promptDel();
				break;
			case YOLO_EDIT:
				//printf("YOLO_EDIT\n");
				if(promptEdit() == -1){
					return -1;
				}
				break;
			case YOLO_EXIT:
				//printf("YOLO_EXIT\n");
				puts_cgc("Thank you for using the yolodex.");
				return 1;
			case YOLO_LIST:
				//printf("YOLO_LIST\n");
				printList();
				break;
			case YOLO_SORT:
				//printf("YOLO_SORT\n");
				sortYolodex();
				break;
			case YOLO_SHOW:
				//printf("YOLO_SHOW\n");
				if(promptShow() == -1){
					return -1;
				}
				break;
			default:
				//printf("default\n");
				puts_cgc("Unrecognized Command.");
		}
	}
	return 0;
}

int main(void)
{
	if(initCookies() != 0){
		return 1;
	}
	//puts_cgc("Welcome to the yolodex.");
	initYolodex();
	if(inputLoop() != 0){
		return 1;
	}
	return 0;
}
