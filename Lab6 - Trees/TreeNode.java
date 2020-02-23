public class TreeNode{
	private Integer value;
	private TreeNode left;
	private TreeNode right;

	public TreeNode(){
		value = null;
		left = null;
		right = null;
	}

	public TreeNode(Integer val){
		value = val;
		left = null;
		right = null;
	}

	public TreeNode(Integer val, TreeNode l, TreeNode r){
		value = val;
		left = l;
		right = r;
	}

	public TreeNode(TreeNode t){
		value = t.value;
		left = t.left;
		right = t.right;
	}

	public Integer getValue(){
		return value;
	}

	public TreeNode getLeft(){
		return left;
	}

	public TreeNode getRight(){
		return right;
	}

	public void setValue(Integer val){
		value = val;
	}

	public void setLeft(TreeNode node){
		left = node;
	}

	public void setRight(TreeNode node){
		right = node;
	}
}