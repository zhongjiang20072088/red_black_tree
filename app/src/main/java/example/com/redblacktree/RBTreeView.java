package example.com.redblacktree;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;

import java.util.Stack;

/**
 * Created by zhongjiang on 17-5-29.
 */

public class RBTreeView extends View {
    private int mRadius;
    private int mXOffset;
    private int mYOffset;
    private Paint mPaint;
    private RBTree mTree;
    private Rect mRect;
    private Stack<RBTree.RBNode> mStack;
    private Stack<RBTree.RBNode> mStackNext;

    public RBTreeView(Context context) {
        this(context, null);
        mStack.clear();
    }

    public RBTreeView(Context context, AttributeSet attrs) {
        super(context, attrs);
        mPaint = new Paint();
        mPaint.setTextAlign(Paint.Align.LEFT);
        mPaint.setTextSize(10);
        mRect = new Rect();
        mStack = new Stack<RBTree.RBNode>();
        mStackNext = new Stack<RBTree.RBNode>();
        mTree = new RBTree();
    }

    public void insert(int key) {
        mTree.insert(key);
        requestLayout();
    }

    public void delete(int key) {
        mTree.delete(key);
        requestLayout();
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        int width = MeasureSpec.getSize(widthMeasureSpec);
        int height = MeasureSpec.getSize(heightMeasureSpec);
        DisplayMetrics dm = getContext().getResources().getDisplayMetrics();
        mRadius = (dm.widthPixels > dm.heightPixels ? dm.heightPixels : dm.widthPixels) / 60;
        mXOffset = (int)(1.1 * mRadius);
        mYOffset = 4 * mXOffset;
        int depth = mTree.depth();
        int treeWidth = (int)(Math.pow(2, depth + 1)) * mXOffset;
        int treeHeight = depth * mYOffset;
        width = width > treeWidth ? width : treeWidth;
        height = height > treeHeight ? height : treeHeight;
        setMeasuredDimension(width, height);
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        mTree.dump();
        mTree.destroy();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (mTree != null) {
            RBTree.RBNode root = mTree.build();
            if (root == null) {
                //Log.d("zhongjiang", "root == null");
                return;
            }

            int depth = mTree.depth();
            //Log.d("zhongjiang", "depth " + depth);
            int x = getWidth() / 2;
            int y = mYOffset;
            root.setX(x);
            root.setY(y);
            mStack.push(root);
            for (int i = 0; i < depth; i++) {
                while(!mStack.empty()) {
                    RBTree.RBNode node = mStack.pop();
                    drawNode(canvas, node);
                    RBTree.RBNode left = node.getLeft();
                    RBTree.RBNode right = node.getRight();
                    if (left != null) {
                        x = node.getX() - (int)(Math.pow(2, (depth - i - 1)))* mXOffset;
                        left.setX(x);
                        y = node.getY() + mYOffset;
                        left.setY(y);
                        mStackNext.push(left);
                    }
                    if (right != null) {
                        x = node.getX() + (int)(Math.pow(2, (depth - i - 1))) * mXOffset;
                        right.setX(x);
                        y = node.getY() + mYOffset;
                        right.setY(y);
                        mStackNext.push(right);
                    }
                }
                mStack.addAll(mStackNext);
                mStackNext.clear();
            }
        }

    }

    private void drawNode(Canvas canvas, RBTree.RBNode node) {
        String key = String.valueOf(node.getKey());
        if (node.getColor() == RBTree.COLOR.RED)
            mPaint.setColor(Color.RED);
        else
            mPaint.setColor(Color.BLACK);
        //Log.d("zhongjiang", "key " + node.getKey() + " x " + node.getX() + " y " + node.getY() + " color " + node.getColor());
        mPaint.setStyle(Paint.Style.FILL);
        int x = node.getX();
        int y = node.getY();
        canvas.drawCircle(x, y, mRadius, mPaint);
        mPaint.setColor(Color.WHITE);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setTextAlign(Paint.Align.LEFT);
        mPaint.getTextBounds(key, 0, key.length() - 1, mRect);
        canvas.drawText(key, x - mRect.width() / 2, y + mRect.height() / 2, mPaint);
        RBTree.RBNode parent = node.getParent();
        if (parent != null) {
            //Log.d("zhongjiang", "parent key " + parent.getKey() + " x " + parent.getX() + " y " + parent.getY() + " color " + parent.getColor());
            mPaint.setColor(Color.BLACK);
            canvas.drawLine(parent.getX(), parent.getY() + mRadius, x, y - mRadius, mPaint);
        }
    }
}
