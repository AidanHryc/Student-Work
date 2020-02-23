public interface HeapPriorityQueueADT{
	public abstract boolean isEmpty();
	public abstract boolean isFull();
	public abstract void enqueue(Integer val)throws HeapOverflowException;
	public abstract Integer dequeue()throws HeapUnderflowException;
	public abstract void reheapifyUpward();
	public abstract void reheapifyDownward();
	public abstract void reposition();
}