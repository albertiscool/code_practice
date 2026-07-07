int binary_search_first(int arr[], int size, int target)
{
    int low = 0;
    int high = size - 1;
    

    while(low <= high)
    {
        int mid = low + ((high - low) >> 1);
        if(arr[mid] == target && arr[mid-1] != target)
        {
            return mid;
        }
        else if(arr[mid] == target && arr[mid-1] == target)
        {
            high = mid - 1;
        }
        else if(arr[mid] > target)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    return -1;
}