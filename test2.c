int X,Y;
int Z = 23;

int swap(int *a, int *b){
	int x = *a;
	int y = *b;
	*a = y;
	*b = x;
	return x + y;	
}
int main(){
	int i = 29;
	int j = 67;
	int sum = swap(&i, &j);
	return 0; 
}
