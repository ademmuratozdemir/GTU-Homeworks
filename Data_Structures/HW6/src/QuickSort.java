public class QuickSort extends SortAlgorithm {

  public QuickSort(int input_array[]) {
    super(input_array);
  }

  // Method to partition the array and return the partition index
  private int partition(int low, int high) {
    // Selecting the pivot element
    int pivot = arr[high];
    int i = low - 1; // Index of smaller element

    // Traverse through all array elements
    for (int j = low; j < high; j++) {
      comparison_counter++; // Increment comparison counter
      // If current element is smaller than the pivot
      if (arr[j] < pivot) {
        i++;
        swap(i, j); // Swap arr[i] and arr[j]
      }
    }

    // Swap arr[i + 1] and arr[high] (or pivot)
    swap(i + 1, high);
    // Return the partition index
    return i + 1;
  }

  // Recursive method to perform quicksort
  private void sort(int low, int high) {
    // If low is smaller than high
    if (low < high) {
      // Get the partition index
      int partition_index = partition(low, high);

      // Recursively sort elements before and after partition
      sort(low, partition_index - 1);
      sort(partition_index + 1, high);
    }
  }

  @Override
  public void sort() {
    // Start the recursive quicksort algorithm
    sort(0, arr.length - 1);
  }

  @Override
  public void print() {
    System.out.print("Quick Sort\t=>\t");
    super.print();
  }
}
