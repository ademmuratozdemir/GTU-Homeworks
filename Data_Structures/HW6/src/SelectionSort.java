public class SelectionSort extends SortAlgorithm {

  public SelectionSort(int input_array[]) {
    super(input_array);
  }

  @Override
  public void sort() {
    // Length of the array
    int n = arr.length;

    // Traverse through all array elements
    for (int i = 0; i < n; i++) {
      // Find the minimum element in unsorted array
      int min_index = i;
      for (int j = i + 1; j < n; j++) {
        comparison_counter++; // Increment comparison counter
        if (arr[j] < arr[min_index]) {
          min_index = j;
        }
      }
      // Swap the found minimum element with the first element
      if (min_index != i) {
        swap(i, min_index);
      }
    }
  }

  @Override
  public void print() {
    System.out.print("Selection Sort\t=>\t");
    super.print();
  }
}
