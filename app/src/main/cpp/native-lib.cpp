#include <jni.h>
#include <string>
#include <assert.h>
#include <android/log.h>
extern "C" {
#include "rb_tree.h"
}
#define TAG    "zhongjiang"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__) // 定义LOGD类型
/*
extern "C"
jstring
Java_example_com_redblacktree_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject */
/* this *//*
) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
*/

#define container_of(ptr, type, member) {( \
	const typeof( (type *0)->member ) __ptr = ptr; \
	(type *)( (char *)__ptr - offsetof(type, memeber) );)}

typedef struct node {
    jobject object;
    rb_node rb_node;
}NODE;

static

static jlong native_init(JNIEnv *env) {
    rb_tree *tree = rb_tree_init();
    if (tree == NULL) {
        return -1;
    } else {
        return reinterpret_cast<jlong>(tree);
    }
}

static jint native_insert(JNIEnv *env, jlong treePtr, jint key) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    return (jint)rb_tree_insert(tree, key);
}

static jint native_delete(JNIEnv *env, jlong treePtr, jint key) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    return (jint)rb_tree_delete(tree, key);
}

static void native_destroy(JNIEnv *env, jlong treePtr) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    rb_tree_destroy(tree);
}

static void native_dump(JNIEnv *env, jlong treePtr) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    rb_tree_dump(tree);

}

static jlong native_tree_root(JNIEnv *env, jlong treePtr) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    if (tree->root == NIL) {
        return 0;
    }
    return reinterpret_cast<jlong >(tree->root);
}

static jint native_tree_depth(JNIEnv *env, jlong treePtr) {
    rb_tree *tree = reinterpret_cast<rb_tree *>(treePtr);
    return (jint)rb_tree_depth(tree);
}

static jlong  native_node_left_child(JNIEnv *env, jlong nodePtr) {
    rb_node *node = reinterpret_cast<rb_node *>(nodePtr);
    if (node->left == NIL) {
        return 0l;
    } else {
        return reinterpret_cast<jlong>(node->left);
    }
}

static jlong native_node_right_child(JNIEnv *env, jlong nodePtr) {
    rb_node *node = reinterpret_cast<rb_node *>(nodePtr);
    if (node->right == NIL) {
        return 0l;
    } else {
        return reinterpret_cast<jlong>(node->right);
    }
}

static jlong native_node_parent(JNIEnv *env, jlong nodePtr) {
    rb_node *node = reinterpret_cast<rb_node *>(nodePtr);
    if (node -> parent == NULL)
        return 0l;
    return reinterpret_cast<jlong>(node->parent);
}

static jint native_node_key(JNIEnv *env, jlong nodePtr) {
    rb_node *node = reinterpret_cast<rb_node *>(nodePtr);
    return node->key;
}

static jint native_node_color(JNIEnv *env, jlong nodePtr) {
    rb_node *node = reinterpret_cast<rb_node *>(nodePtr);
    return node->color;
}

static const char *tree_class_path = "example/com/redblacktree/RBTree";

static const char *node_class_path = "example/com/redblacktree/RBTree$RBNode";

static JNINativeMethod tree_methods[] = {
        {"native_init", "()J", (void *)native_init},
        {"native_insert", "(JI)I", (void *)native_insert},
        {"native_delete", "(JI)I", (void *)native_delete},
        {"native_destroy", "(J)V", (void *)native_destroy},
        {"native_dump", "(J)V", (void *)native_dump},
        {"native_tree_root", "(J)J", (void *)native_tree_root},
        {"native_tree_depth", "(J)I", (void *)native_tree_depth}
};

static JNINativeMethod node_methods[] = {
        {"native_node_left_child", "(J)J", (void *)native_node_left_child},
        {"native_node_right_child", "(J)J", (void *)native_node_right_child},
        {"native_node_parent", "(J)J", (void *)native_node_parent},
        {"native_node_key", "(J)I", (void *)native_node_key},
        {"native_node_color", "(J)I", (void *)native_node_color}
};

static int registerNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *gMethods, int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == NULL)
        return JNI_FALSE;
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static int registerNatives(JNIEnv *env) {
    if (!registerNativeMethods(env, tree_class_path, tree_methods, sizeof(tree_methods) / sizeof(tree_methods[0]))) {
        return JNI_FALSE;
    }
    if (!registerNativeMethods(env, node_class_path, node_methods, sizeof(node_methods) / sizeof(node_methods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    jint result = -1;
    LOGD("JNI_OnLoad");
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        LOGD("JNI_OnLoad 1");
        if (env == NULL)
            LOGD("env == NULL");
        goto bail;
    }

    if (registerNatives(env) == JNI_TRUE) {
        LOGD("JNI_OnLoad 2");
        result = JNI_VERSION_1_4;
        goto bail;
    }
    LOGD("JNI_OnLoad 3");
    bail:
    return result;
}





