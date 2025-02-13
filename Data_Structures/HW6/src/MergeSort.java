public class MergeSort extends SortAlgorithm {

  public MergeSort(int input_array[]) {
    super(input_array);
  }

  // Method to merge two subarrays of arr[]
  private void merge(int left, int middle, int right) {
    // Sizes of two subarrays to be merged
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temporary arrays
    int L[] = new int[n1];
    int R[] = new int[n2];

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; ++i) L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j) R[j] = arr[middle + 1 + j];

    // Initial indexes of first and second subarrays
    int i = 0, j = 0;

    // Initial index of merged subarray array
    int k = left;
    // Merge the temporary arrays
    while (i < n1 && j < n2) {
      comparison_counter++; // Increment comparison counter
      if (L[i] <= R[j]) {
        arr[k] = L[i];
        i++;
      } else {
        arr[k] = R[j];
        j++;
      }
      k++;
    }

    // Copy remaining elements of L[] if any
    while (i < n1) {
      arr[k] = L[i];
      i++;
      k++;
    }

    // Copy remaining elements of R[] if any
    while (j < n2) {
      arr[k] = R[j];
      j++;
      k++;
    }
  }

  // Recursive method to perform merge sort
  private void sort(int left, int right) {
    // If left is smaller than right
    if (left < right) {
      // Find the middle point
      int middle = (left + right) / 2;

      // Sort first and second halves
      sort(left, middle);
      sort(middle + 1, right);

      // Merge the sorted halves
      merge(left, middle, right);
    }
  }

  @Override
  public void sort() {
    // Start the recursive merge sort algorithm
    sort(0, arr.length - 1);
  }

  @Override
  public void print() {
    // Print the name of the sorting algorithm
    System.out.print("Merge Sort\t=>\t");
    super.print();
  }
}
