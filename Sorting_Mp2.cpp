#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

#define swap(a,b) {int t;t=a;a=b;b=t;}
int arr_temp[5] = { 0 };
//insertionsort
void insertionSort(int *arr, int n) {
	int i, j;
	int temp;
	for (i = 2; i < n; i++) {
		temp = arr[i];
		j = i;
		while ((j > 1) && (temp < arr[j - 1])) {
			arr[j] = arr[j - 1];
			j--;
		}
		arr[j] = temp;
	}
}

//pivot을 중심으로 수를 나눠주는 역할
int partition(int*arr, int low, int high) {
	int pivot = low;
	for (int i = low; i < high; i++) {
		if (arr[i] < arr[high]) {
			swap(arr[i], arr[pivot]);
			pivot++;
		}
	}
	swap(arr[pivot], arr[high]);
	return pivot;
}

//quick sort
void quickSort(int *arr, int low, int high) {
	int pi;
	if (low < high) {
		pi = partition(arr, low, high);
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

//두 그룹에 대해 수를 하나씩 비교해서 정렬해줌
void merge(int *arr, int start, int middle, int end, int n) {
	int tempStart = middle + 1;
	int t = start;
	int t_start = start;
	int *tempArr = (int*)malloc(sizeof(int)*(n+1));
	for (int i = start; i < (end + 1); i++) tempArr[i] = arr[i];
	while ((t_start <= middle) && (tempStart <= end)) {
		if (tempArr[t_start] < tempArr[tempStart]) arr[t++] = tempArr[t_start++];
		else arr[t++] = tempArr[tempStart++];
	}
	while (t_start <= middle) arr[t++] = tempArr[t_start++];
	while (tempStart <= end) arr[t++] = tempArr[tempStart++];
	free(tempArr);
}

//merge sort
void mergeSort(int*arr, int start, int end,int n) {
	int middle = (end + start) / 2;
	if (start < end) {
		mergeSort(arr, start, middle, n);
		mergeSort(arr, middle + 1, end, n);
		merge(arr, start, middle, end, n);
	}
}

//radix sort
void radixSort(int *arr, int n, int max) {
	int*b = (int*)malloc(sizeof(int)*(n + 1));
	int bin_count[10] = { 0 };
	int sig_digit = 1, i;
	while (max / sig_digit > 0) {
		for (i = 0; i < 10; i++) bin_count[i] = 0;
		for (i = 1; i <= n; i++) bin_count[(arr[i] / sig_digit) % 10]++;
		for (i = 1; i < 10; i++) bin_count[i] += bin_count[i - 1];
		for (i = n; i >= 1; i--) b[bin_count[(arr[i] / sig_digit) % 10]--] = arr[i];
		for (i = 1; i <= n; i++) arr[i] = b[i];
		sig_digit *= 10;
	}
	free(b);
}

//root부터 n까지 배열에서 arr[root]값의 올바른 위치를 찾아줌
void adjust(int*arr, int root, int n) {
	int rootkey = arr[root];
	int child = root * 2;
	while (child <= n) {
		if ((child < n) && (arr[child] < arr[child + 1])) child++;
		if (rootkey > arr[child]) break;
		arr[child / 2] = arr[child];
		child *= 2;
	}
	arr[child / 2] = rootkey;
}

//heap을 사용한 정렬, adjust를 활용해서 오름차순으로 정렬한다.
void heapSort(int *arr, int size) {
	int i;
	int n = size;
	for (i = size / 2; i > 0; i--) {
		adjust(arr, i, size);
	}
	for (i = size; i > 1; i--) {
		swap(arr[i], arr[1]);
		adjust(arr, 1, i - 1);
	}
}
//quicksort를 몇 번 실행한 뒤 sorting이 완료되지 않았을 경우 heapsort로 정렬
void introSort(int*arr, int low, int high, int max_depth, int size) {
	int n = high - low + 1;
	if (n <= 1) return;
	else if (max_depth == 0) heapSort(arr, size);
	else {
		int pi = partition(arr, low, high);
		introSort(arr, low, pi - 1, max_depth - 1, size);
		introSort(arr, pi + 1, high, max_depth - 1, size);
	}
}

//stack을 활용한 iteration quick sort
//quick sort의 비효율성을 해결하고자 임의의 3개의 수를 비교한 뒤 중간 값을 pivot으로 사용
void quick_iteration(int *arr, int low, int high) {
	int*stack = (int*)malloc(sizeof(int)*(high - low + 2));
	int r[3] = { 0 };
	int i, l, h, pi, pi_temp, top = 0;
	stack[++top] = low;
	stack[++top] = high;
	while (top) {
		h = stack[top--];
		l = stack[top--];
		pi = l;
		if (h - l > 5) {
			r[0] = rand() % (h - l + 1) + l;
			r[1] = rand() % (h - l + 1) + l;
			r[2] = rand() % (h - l + 1) + l;
			//3개의 index중 arr에서 중간값인 index구하기
			pi_temp = arr[r[0]] > arr[r[2]] ? (arr[r[2]] > arr[r[1]] ? r[2] : (arr[r[1]] > arr[r[0]] ? r[0] : r[1])) : (arr[r[0]] > arr[r[1]] ? r[0] : (arr[r[1]] > arr[r[2]] ? r[2] : r[1]));
			swap(arr[pi_temp], arr[h]);
		}
		for (i = l; i < h; i++) {
			if (arr[i] < arr[h]) {
				swap(arr[i], arr[pi]);
				pi++;
			}
		}
		swap(arr[h], arr[pi]);
		if (pi - l > 1) {
			stack[++top] = l;
			stack[++top] = pi - 1;
		}
		if (h - pi > 1) {
			stack[++top] = pi + 1;
			stack[++top] = h;
		}
	}
	free(stack);
}

void myown(int *arr, int n, int max, int min) {
	int i;
	double check1, check2;

	//iteration을 사용한 quicksort와 radixsort의 시간복잡도를 비교
	check1 = n * (log10(n) / log10(2));
	check2 = 5 * n * (ceil(log10(max)) + 1);

	//소수가 없고 radix sort의 시간복잡도가 더 적을 때 실행
	//모든 수를 양의 정수로 만들고 sorting한 뒤 원래 수로 바꿔서 본 arr에 저장
	if (check1 > check2) {
		for (i = 1; i <= n; i++) arr[i] -= min;
		radixSort(arr, n, max-min);
		for (i = 1; i <= n; i++) arr[i] += min;
	}
	else {
		quick_iteration(arr, 1, n);
	}
}

int main(int argc, char*argv[]) {
	clock_t start, end;
	double duration;
	FILE*fp1, *fp2;		//fp1: 읽어 들이는 파일 포인터, fp2: 결과 저장하는 파일 포인터
	int n, i;
	int *arr;
	int max = 0, min = 0;
	char fileName[100] = "result_";

	//radix sort를 위한 작업도 같이함 소수가 포함되어있는지 확인하고 max와 min(음수 대비)을 미리 구함
	fp1 = fopen(argv[1], "r");
	fscanf(fp1, "%d", &n);
	arr = (int*)malloc(sizeof(int)*(n + 1));
	for (i = 1; i <= n; i++) {
		fscanf(fp1, "%d", &arr[i]);
		if (arr[i] > max) max = arr[i];
		if (arr[i] < min) min = arr[i];
	}
	fclose(fp1);

	//sorting 부분
	start = clock();
	if (*argv[2] == '1') insertionSort(arr, n + 1);
	else if (*argv[2] == '2') quickSort(arr, 1, n);
	else if (*argv[2] == '3') heapSort(arr, n);
	else if (*argv[2] == '4') myown(arr, n, max, min);
	else printf("algorithm number must be 1,2,3,4");
	end = clock();
	duration = (double)(end - start) / CLOCKS_PER_SEC;

	//결과 저장
	strcat(fileName, argv[2]);
	strcat(fileName, "_");
	strcat(fileName, argv[1]);
	fp2 = fopen(fileName, "w");
	fprintf(fp2, "%s\n%c\n%d\n%.6f\n", argv[1], *argv[2], n, duration);
	for (i = 1; i <= n; i++) fprintf(fp2, "%d ", arr[i]);
	fclose(fp2);
	free(arr);
}