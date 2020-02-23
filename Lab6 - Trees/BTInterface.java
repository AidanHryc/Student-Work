public interface BTInterface{
	abstract Integer getRoot();
	abstract void setRoot(TreeNode t);
	abstract boolean isEmpty();
	abstract void swapSubtrees();
	abstract int singleParent();
	abstract void preOrder();
	abstract void postOrder();
	abstract void inOrder();
	abstract void insert(Integer val);
}