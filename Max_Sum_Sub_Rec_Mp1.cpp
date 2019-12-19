#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

//get max subsequence sum of arr
int getMax(int arr[],int r) {
	int i, thisSum=0, max=-100000;
	for (i = 0; i < r; i++) {
		thisSum += arr[i];
		if (thisSum < 0) thisSum = 0;
		else if (thisSum > max) max = thisSum;
	}
	return max;
}

int main(int argc,char*argv[])
{
	clock_t start, end;
	double duration;
	char fileName[100] = "result_";

	FILE*fp = fopen(argv[1], "r");			//"a.txt"-> *argv[1]
	int r, c,i,j,k,flag=1;					//flag-> 양수가 하나라도 존재하면 0이됨
	int i1, i2, j1, j2, k1, k2;				//i1,i1->전체 행렬을 움직이는 변수(행,열) , j1,j2->부분 행렬을 움직이는 변수(행,열)
	int sum,sumb,max=-100000;				//sumb-> 이전 직사각형 합산값(sum before)
	char trash;								//엔터키 읽는 변수
	int**mat,**mat2;						//mat-> 원래 행렬값 저장, mat2-> 세로합 저장
	int *arr;								//O(n^3)에 쓰임
	fscanf(fp,"%d %d%c",&r,&c,&trash);
		
	mat = (int**)malloc(sizeof(int*)*r);
	mat2 = (int**)malloc(sizeof(int*)*r);
	arr = (int*)malloc(sizeof(int)*r);

	for (i = 0; i < r; i++) {
		mat[i] = (int*)malloc(sizeof(int)*c);
		mat2[i] = (int*)malloc(sizeof(int)*c);
		for (j = 0; j < c; j++) {
			fscanf(fp,"%d", &mat[i][j]);
			if (mat[i][j] > 0) flag = 0;
			if (i == 0) mat2[i][j] = mat[i][j];
		}
		fscanf(fp, "%c", &trash);
	}
	//모든 수가 음수일 때 가장 큰 음수 찾음
	if (flag) {
		start = clock();
		for (i = 0; i < r; i++) {
			for (j = 0; j < c; j++) {
				if (mat[i][j] > max) max = mat[i][j];
			}
		}
		end = clock();
	}
	//O(n^6)
	else if(*argv[2]=='1') {
		start = clock();
		for (i1 = 0; i1 < r; i1++) {
			for (i2 = 0; i2 < c; i2++) {
				for (j1 = i1; j1 < r; j1++) {
					for (j2 = i2; j2 < c; j2++) {
						sum = 0;
						for (k1 = i1; k1 <= j1; k1++) {
							for (k2 = i2; k2 <= j2; k2++) {
								sum += mat[k1][k2];
							}
						}
						if (sum > max) max = sum;
					}
				}
			}
		}
		end = clock();
	}
	//O(n^4)
	else if (*argv[2] == '2') {
		start = clock();
		for (i1 = 0; i1 < r; i1++) {
			for (i2 = 0; i2 < c; i2++) {
				for (j1 = i1; j1 < r; j1++) {
					sum = 0;
					for (j2 = i2; j2 < c; j2++) {
						if (j2 == i2 && i1 == j1) sum = mat[j1][j2];
						else if (i1 == j1) sum = sumb + mat[j1][j2];
						else if (i2 == j2) sum = mat[j1][j2] + mat2[j1 - 1][j2];
						else sum = mat[j1][j2] + mat2[j1 - 1][j2] + sumb;
						sumb = sum;
						if (i1 != j1) mat2[j1][j2] = mat2[j1 - 1][j2] + mat[j1][j2];
						else mat2[j1][j2] = mat[j1][j2];
						if (sum > max) max = sum;
					}
				}
			}
		}
		end = clock();
	}
	//O(n^3)
	else if(*argv[2]=='3') {
		start = clock();
		for (k = 0; k < c; k++) {
			for (j = 0; j < r; j++)
				arr[j] = 0;
			for (j = k; j < c; j++) {
				for (i = 0; i < r; i++) {
					arr[i] += mat[i][j];
				}
				sum=getMax(arr,r);
				if (sum > max) max = sum;
			}
		}
		end = clock();
	}
	else{
		printf("올바른 숫자를 입력해주세요\n");
		fclose(fp);
		for (i = 0; i < r; i++) {
                	free(mat[i]);
                	free(mat2[i]);
        	}
        	free(mat);
        	free(mat2);
        	free(arr);
		return 0;
	}
	duration = (double)(end - start) / CLOCKS_PER_SEC;
	fclose(fp);

	strcat(fileName, argv[1]);
	fp = fopen(fileName, "w");
	fprintf(fp, "%s\n%c\n%d\n%d\n%d\n%.1f", argv[1], *argv[2], r, c, max, duration);
	fclose(fp);

	for (i = 0; i < r; i++) {
		free(mat[i]);
		free(mat2[i]);
	}
	free(mat);
	free(mat2);
	free(arr);
}
