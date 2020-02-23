public class HeapPriorityQueue implements HeapPriorityQueueADT{
	private Integer heapArr[];
	private int count;

	public HeapPriorityQueue(){
		heapArr = new Integer[250];
		count = 0;
	}

	public boolean isEmpty(){
		return (count == 0);
	}

	public boolean isFull(){
		return (count == 250);
	}

	public void enqueue(Integer val)throws HeapOverflowException{
		if(count == 250)
			throw new HeapOverflowException();

		heapArr[count] = val;
		reheapifyUpward();
		count++;
	}

	public Integer dequeue()throws HeapUnderflowException{
		if(count == 0)
			throw new HeapUnderflowException();

		int temp = heapArr[0];
		reposition();
		reheapifyDownward();
		--count;
		return temp;
	}

	public void reposition(){
		heapArr[0] = heapArr[count-1];
	}

	public void reheapifyUpward(){
		int temp = 0;
		int parentIndex = (count-1)/2;
		int curIndex = count;
		while(heapArr[parentIndex] > heapArr[curIndex]){
			temp = heapArr[parentIndex];
			heapArr[parentIndex] = heapArr[curIndex];
			heapArr[curIndex] = temp;
			curIndex = parentIndex;
			parentIndex = (parentIndex-1)/2;
		}
	}

	public void reheapifyDownward(){
		int curIndex = 0;
		int temp = 0;
		Integer childIndex = 0;

		if(heapArr[1] != null && heapArr[2] != null)
			childIndex = (heapArr[1]<heapArr[2])? 1 : 2;
		else if(heapArr[1] != null)
			childIndex = 1;
		else
			childIndex = null;

		while(childIndex != null && childIndex < count ){
			temp = heapArr[childIndex];
			heapArr[childIndex] = heapArr[curIndex];
			heapArr[curIndex] = temp;
			curIndex = childIndex;

			//Picking the smaller of the two children
			if(heapArr[childIndex*2+1] != null && heapArr[childIndex*2+2] != null)
				childIndex = (heapArr[curIndex*2+1]<heapArr[curIndex*2+2])? curIndex*2+1 : curIndex*2+2;
			else if(heapArr[childIndex*2+1] != null)
				childIndex = childIndex*2+1;
			else
				childIndex = null;
		}
	}

	public String toString(){
		int lvlCount = 1;
		String str = "";

		str += "Level 0:\n";
		for(int i = 0; i<count; i++){
			if(lvlCount == (Math.log(i+1)/Math.log(2))){
				str += "\nLevel "+lvlCount+":\n";
				str += heapArr[i] + " ";
				lvlCount++;
			}
			else{
				str += heapArr[i] + " ";
			}
		}
		return str;
	}
}