
char ANS[5];

void func(char *str){
	ANS[0] = str[0];
	ANS[1] = str[1];
	ANS[2] = str[2];
	ANS[3] = str[3];
	ANS[4] = str[4];
}

int main(){
	char s[] = "Hello";
	func(s);
	return 0;
}
