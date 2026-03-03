#include <jni.h>
#include <string>


//NDK工具链里面的Log库
#include <android/log.h>
#define TAG  "LOGAN"
//...表示不知道传入什么，借助JNI里的宏来自动帮我填充
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG,__VA_ARGS__)



extern "C" //全部采用C的方式，不准你的函数重载，函数名一样的问题
JNIEXPORT //标记该方法可以被外部调用
jstring  //Java<----->native 转换用的
JNICALL //代表JNI标记
//JNIEnv* env JNI的桥梁环境 300多个函数，所有JNI操作
//jobject 谁调用就是谁的实例 MainActivity this
//jclass clazz 谁调用，就是谁的class MainActivity.class
//函数的实现
Java_com_example_as_1jni_1project_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//静态
extern "C"
JNIEXPORT jstring JNICALL
Java_com_example_as_1jni_1project_MainActivity_StringFromJNI2(JNIEnv *env, jclass clazz) {
    // TODO: implement StringFromJNI2()
}

//修改变量名
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_changeName(JNIEnv *env, jobject thiz) {
    //获取class
    jclass j_cls = env->GetObjectClass(thiz);
    //获取属性 L对象类型 都需要L
    //GetFieldID(MainActivity.class,属性名,属性的签名)
    jfieldID j_fid = env->GetFieldID(j_cls,
                                     "name","Ljava/lang/String;");
    //转换工作
    jstring j_str = static_cast<jstring>(env->GetObjectField(thiz,j_fid));
    //打印字符串 目标
    char * c_str = const_cast<char *>(env->GetStringUTFChars(j_str, NULL));
    LOGD("native:%s\n",c_str);
    LOGE("native:%s\n",c_str);
    LOGI("native:%s\n",c_str);
    //修改成Beyond
    jstring jName = env->NewStringUTF("Beyond");
    //要用jString进行操作
    env->SetObjectField(thiz,j_fid,jName);

}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_changeAge(JNIEnv *env, jclass clazz) {
    const char* sign = "I";
    const char* name = "age";
    jfieldID j_fid = env->GetStaticFieldID(clazz,name,sign);
    jint  age = env->GetStaticIntField(clazz,j_fid);
    age+=10;
    env->SetStaticIntField(clazz,j_fid,age);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_callAddMethod(JNIEnv *env, jobject thiz) {
    //自己得到MainActivity
    jclass mainActivityClass = env->GetObjectClass(thiz);
    //GetMethodId(MainActivity.class，方法名，方法名的签名)
    jmethodID jmid = env->GetMethodID(mainActivityClass,"add","(II)I");
    //调用java的方法
    jint sum = env->CallIntMethod(thiz,jmid,3,3);
    LOGE("sum result:%d",sum);
}