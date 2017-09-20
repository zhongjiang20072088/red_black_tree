package example.com.redblacktree;

import android.graphics.Canvas;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhongjiang on 17-5-18.
 */

public class RBTree {

    static {
        try {
            System.loadLibrary("native-lib");
        }catch (UnsatisfiedLinkError e) {
            e.printStackTrace();
            throw new RuntimeException(e.getMessage());
        }

    }

    private static final List<RBNode> sPool = new ArrayList<RBNode>();
    private List<RBNode> mPool;
    private long mRBTreePtr;
    private native long native_init();
    private native int native_insert(long treePtr, int key);
    private native int native_delete(long treePtr, int key);
    private native void native_destroy(long treePtr);
    private native void native_dump(long treePtr);
    private native long native_tree_root(long treePtr);
    private native int native_tree_depth(long treePtr);

    public RBTree() {
        mRBTreePtr = native_init();
        mPool = new ArrayList<RBNode>();
    }

    public void insert(int key) {
        Log.d("zhongjiang", "insert " + key);
        native_insert(mRBTreePtr, key);
    }

    public void dump() {
        native_dump(mRBTreePtr);
    }

    public void delete(int key) {
        native_delete(mRBTreePtr, key);
    }

    public void destroy() {
        native_destroy(mRBTreePtr);
    }

    public int depth() {
        return native_tree_depth(mRBTreePtr);
    }

    public RBNode build() {
        long rootPtr = native_tree_root(mRBTreePtr);
        if (rootPtr != 0) {
             sPool.addAll(mPool);
             mPool.clear();
            RBNode root = RBTree.this.newRBNode(rootPtr);
            root.setParent(null);
             return root;
        } else {
            return null;
        }
    }


    private RBNode newRBNode(long nodePtr) {
        RBNode node;
        if (sPool.size() > 0) {
            node = sPool.remove(0);
            node.init(nodePtr);
        } else {
            node = new RBNode(nodePtr);
            mPool.add(node);
        }
        return node;
    }
    public enum COLOR {RED, BLACK}
    public class RBNode {

        private int key;
        private int x;
        private int y;
        private long nodePtr;
        private COLOR color;
        private RBNode left;
        private RBNode right;
        private RBNode parent;

        private RBNode(long nodePtr) {
            init(nodePtr);
        }
        private native long native_node_left_child(long nodePtr);
        private native long native_node_right_child(long nodePtr);
        private native long native_node_parent(long nodePtr);
        private native int native_node_key(long nodePtr);
        private native int native_node_color(long nodePtr);

        public void init(long nodePtr) {
            this.nodePtr = nodePtr;
            //Log.d("zhongjiang", "nodePtr " + nodePtr);
            setKey(native_node_key(nodePtr));
            //Log.d("zhongjiang", "init key " + getKey() + " color " + getColor());
            //Log.d("zhongjiang", "set key " + native_node_key(nodePtr));
            if(native_node_color(nodePtr) == 0)
                setColor(COLOR.RED);
            else
                setColor(COLOR.BLACK);
            long leftNodePtr = native_node_left_child(nodePtr);
            if (leftNodePtr != 0) {
                //Log.d("zhongjiang", "leftNodePtr " + leftNodePtr);
                RBNode left = RBTree.this.newRBNode(leftNodePtr);
                setLeft(left);
                left.setParent(this);
                //Log.d("zhongjiang", "this key " + getKey() + " left " + left.getKey());
            } else {
                setLeft(null);
                //Log.d("zhongjiang", "this key " + getKey() + " left " + null);
            }
            long rightNodePtr = native_node_right_child(nodePtr);
            //Log.d("zhongjiang", "rightNodePtr " + rightNodePtr);
            if (rightNodePtr != 0) {
                RBNode right = RBTree.this.newRBNode(rightNodePtr);
                setRight(right);
                right.setParent(this);
                //Log.d("zhongjiang", "this key " + getKey() + " right " + left.getKey());
            } else {
                setRight(null);
                //Log.d("zhongjiang", "this key " + getKey() + " right " + null);
            }
            //Log.d("zhongjiang", "init key " + getKey() + " x " + getX() + " y " + getY() + " color " + getColor());
        }

        public void setKey(int key) {
            this.key = key;
        }

        public int getKey() {
            return this.key;
        }

        public void setLeft(RBNode left) {
            this.left = left;
        }

        public RBNode getLeft() {
            return this.left;
        }

        public void setRight(RBNode right) {
            this.right = right;
        }

        public RBNode getRight() {
            return this.right;
        }

        public void setParent(RBNode parent) {
            this.parent = parent;
        }

        public RBNode getParent() {
            return this.parent;
        }

        public void setX(int x) {
            this.x = x;
        }

        public void setY(int y) {
            this.y = y;
        }

        public int getX() {
            return this.x;
        }

        public int getY() {
            return this.y;
        }

        public void setColor(COLOR color) {
            this.color = color;
        }

        public COLOR getColor() {
            return this.color;
        }
    }

    private final static class Stack extends ArrayList<RBNode>{

        public RBNode top() {
            assert(!isEmpty());
            return get(size() -1);
        }

        public void pop() {
            assert(!isEmpty());
            remove(size() - 1);
        }

        public void push(RBNode node) {
            add(size(), node);
        }
    }

}


