int binary_search_first(int arr[], int size, int target)
{
    int low = 0;
    int high = size - 1;
    int result = -1;

    while (low <= high)
    {
        // 1. 完美的防溢位公式
        int mid = low + ((high - low) >> 1);
        
        // 2. 互斥的三分法邏輯
        if (arr[mid] == target)
        {
            result = mid;       // 找到了！先存起來
            high = mid - 1;     // 不要停，繼續往左邊找有沒有更早出現的
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return result;
}