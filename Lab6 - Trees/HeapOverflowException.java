public class HeapOverflowException extends Exception{
	public HeapOverflowException(){
		super("HeapOverflowException: Full Heap");
	}

	public HeapOverflowException(String msg){
		super(msg);
	}
}