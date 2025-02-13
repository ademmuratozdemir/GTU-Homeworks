public class BubbleSort extends SortAlgorithm {

  public BubbleSort(int input_array[]) {
    super(input_array);
  }

  @Override
  public void sort() {
    // Length of the array
    int n = arr.length;
    boolean swapped;

    // Traverse through all array elements
    for (int i = 0; i < n - 1; i++) {
      swapped = false;
      // Last i elements are already in place, so we do not need to check them
      for (int j = 0; j < n - i - 1; j++) {
        comparison_counter++; // Increment comparison counter
        // Swap if the element found is greater than the next element
        if (arr[j] > arr[j + 1]) {
          swap(j, j + 1);
          swapped = true;
        }
      }
      // If no two elements were swapped in the inner loop, then the array is already sorted
      if (!swapped) {
        break;
      }
    }
  }

  @Override
  public void print() {
    System.out.print("Bubble Sort\t=>\t");
    super.print();
  }
}
