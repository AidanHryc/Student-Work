public class HeapUnderflowException extends Exception{
	public HeapUnderflowException(){
		super("HeapUnderflowException: Empty Heap");
	}

	public HeapUnderflowException(String msg){
		super(msg);
	}
}