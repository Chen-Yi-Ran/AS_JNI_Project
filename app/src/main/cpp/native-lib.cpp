#include <jni.h>
#include <string>
#include "iostream"

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

//jint == int
//jstring == String
//jintArray == int[]
//jobjectArray == 引用类型对象，例如String[] Test[] Student[] Person[]
extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_testArrayAction(JNIEnv *env, jobject thiz,
                                                               jint count,
                                                               jstring text_indo,
                                                               jintArray ints,
                                                               jobjectArray strs) {
    //基本数据类型 jint count,jstring text_info，最简单的
    int countInt = count; //jint本质是int，所以可以用int接收
    LOGI("参数一 countInt：%d\n",count);

    //const char* GetStringUTFChars(jstring string,jboolean* isCopy)
    const char* testInfo = env->GetStringUTFChars(text_indo,NULL);
    LOGI("参数二 textInfo:%s\n",testInfo);

    // ② 把int[] 转成 int*
    // jint* GetIntArrayElements(jintArray array, jboolean* isCopy)
    int* jintArray = env->GetIntArrayElements(ints,NULL);
    //Java层数组的长度
    // jsize GetArrayLength(jarray array) // jintArray ints 可以放入到 jarray的参数中去
    jsize size = env->GetArrayLength(ints);
    for(int i = 0;i<size;i++){
        *(jintArray+i)+=100;
        LOGI("参数三 int[]:%d\n",*jintArray+i);
    }
    // 目前无法控制Java的数组 变化 +100
    // 操作杆 ----&gt; JMV
    // env-&gt;

    /**
     * 0:           刷新Java数组，并 释放C++层数组
     * JNI_COMMIT:  只提交 只刷新Java数组，不释放C++层数组
     * JNI_ABORT:   只释放C++层数组
     */
     env->ReleaseIntArrayElements(ints,jintArray,0);


     //jobjectArray代表是Java的引用类型数组，不一样
     jsize strSize = env->GetArrayLength(strs);
     for(int i = 0;i<strSize;i++){
         jstring jobj = static_cast<jstring>(env->GetObjectArrayElement(strs, i));
         // 模糊：isCopy内部启动的机制
         // const char* GetStringUTFChars(jstring string, jboolean* isCopy)
         const char * jobjCharp = env->GetStringUTFChars(jobj,NULL);
         LOGI("参数四 引用类型String具体的：%s\n",jobjCharp);
         //释放jString
         env->ReleaseStringUTFChars(jobj,jobjCharp);
     }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_putObject(JNIEnv *env,
                                                         jobject thiz,
                                                         jobject student,
                                                         jstring str) {
    const char* strChar = env->GetStringUTFChars(str,NULL);
    LOGI("strChar:%s\n",strChar);
    env->ReleaseStringUTFChars(str,strChar);

    //---------
    //1.寻找类Student
    //jclass studentClass = env->FindClass("com/derry/as_jni_project/Student"); // 第一种
    jclass studentClass = env->GetObjectClass(student); //第二种

    //2.Student类里面的函数规则 签名
    jmethodID  setName = env->GetMethodID(studentClass,"setName","(Ljava/lang/String;)V");
    jmethodID getName = env->GetMethodID(studentClass,"getName","()Ljava/lang/String;");
    jmethodID showInfo = env->GetStaticMethodID(studentClass,"showInfo","(Ljava/lang/String;)V");

    //调用setName
    jstring value = env->NewStringUTF("AAAA");
    env->CallVoidMethod(student,setName,value);

    //调用getName
    jstring getNameResult = static_cast<jstring>(env->CallObjectMethod(student, getName));
    const char * getNameValue = env->GetStringUTFChars(getNameResult,NULL);
    LOGE("调用到getName方法，值是:%s\n",getNameValue);

    //调用静态showInfo
    jstring jstringValue = env->NewStringUTF("静态方法你好，我是C++");
    env->CallStaticVoidMethod(studentClass,showInfo,jstringValue);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_as_1jni_1project_MainActivity_insertObject(JNIEnv *env, jobject thiz) {

    //1.通过包名+类名的方式，拿到Student class，凭空拿class
    const char * studentStr = "com/example/as_jni_project/Student";
    jclass studentClas = env->FindClass(studentStr);

    //2.通过student的class实例化此Student对象 c++ new Student
    jobject studentObj = env->AllocObject(studentClas);//AllocObject只实例化对象，不会调用对象的构造函数
    //方法的签名规则
    jmethodID setName = env->GetMethodID(studentClas,"setName","(Ljava/lang/String;)V");
    jmethodID setAge = env->GetMethodID(studentClas,"setAge","(I)V");

    //调用方法
    jstring strValue = env->NewStringUTF("Logan");
    env->CallVoidMethod(studentObj,setName,strValue);
    env->CallVoidMethod(studentObj,setAge,99);
    // env-&gt;NewObject() // NewObject 实例化对象，会调用对象的构造函数
    // ====================  下面是 Person对象  调用person对象的  setStudent 函数等
    //4.通过包名+类名的方式，拿到Student class 凭空拿class
    const char *personstr = "com/example/as_jni_project/Person";
    jclass personClass = env->FindClass(personstr);
    jobject personObj  = env->AllocObject(personClass); //AllocObject 只实例化对象，不会调用对象的构造函数
    //setStudent此函数的签名规则
    jmethodID  setStudent = env->GetMethodID(personClass,"setStudent","(Lcom/example/as_jni_project/Student;)V");
    env->CallVoidMethod(personObj,setStudent,studentObj);
}