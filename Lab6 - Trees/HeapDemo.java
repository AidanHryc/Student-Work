public class HeapDemo{
	public static void main(String...args)throws HeapOverflowException, HeapUnderflowException{
		HeapPriorityQueue heap = new HeapPriorityQueue();
		for(int i = 1; i<=10; i++){
			heap.enqueue(i);
		}
		System.out.println(heap);
		System.out.println();
		System.out.println("Remove: "+heap.dequeue());
		System.out.println("Remove: "+heap.dequeue());
		System.out.println("Remove: "+heap.dequeue());
		System.out.println();
		System.out.println(heap);

	}
}
