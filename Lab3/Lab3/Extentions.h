#include "List.h"


namespace Extentions {
	template <typename T, typename SortCondition>
	void TopDownMerge(List<T> &t, int iBegin, int iMiddle, int iEnd, List<T> &array, SortCondition cond)
	{
		int i = iBegin, j = iMiddle;
		for (int k = iBegin; k < iEnd; k++) {
			//Stability!
			if (i < iMiddle && (j >= iEnd || !cond(array[j], array[i]))) {
				t[k] = array[i];
				i++;
			}
			else {
				t[k] = array[j];
				j++;
			}
		}
	}
	template <typename T, typename SortCondition>
	void TopDownSplitMerge(List<T> &t, int iBegin, int iEnd, List<T> &array, SortCondition cond)
	{
		if (iEnd - iBegin <= 1)
			return;

		int iMiddle = (iEnd + iBegin) / 2;

		TopDownSplitMerge(array, iBegin, iMiddle, t, cond);
		TopDownSplitMerge(array, iMiddle, iEnd, t, cond);
		TopDownMerge(t, iBegin, iMiddle, iEnd, array, cond);
	}

	template <typename T, typename SortCondition>
	void MergeSort(List<T> &array, SortCondition cond)
	{
		List<T>* t = array.Copy();
		TopDownSplitMerge(array, 0, array.size(), *t, cond);
	}

}