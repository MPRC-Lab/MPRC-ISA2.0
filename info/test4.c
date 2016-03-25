
int ANS;
int SORT[5];
int fab(int num){
	if(num == 1)
		return 1;
	return fab(num-1) + num;
}

void quick(int l, int r, int num[]){
	if(l < r){
		int i = l, j = r, x = num[l];
		while(i < j){
			while(i <j && num[j] >= x) j--;
			if(i < j) num[i++] = num[j];
			while(i < j && num[i] < x) i++;
			if(i < j) num[j--] = num[i];
		}
		num[i] = x;
		quick(l, i-1, num);
		quick(i+1, r, num);
	}
}

int main(){
	int sortNum[5] = {5,4,7,2,0};
	quick(0,4, sortNum);
		SORT[0] = sortNum[0];
		SORT[1] = sortNum[1];
		SORT[2] = sortNum[2];
		SORT[3] = sortNum[3];
		SORT[4] = sortNum[4];

	ANS = fab(20);
	return 0;
}
