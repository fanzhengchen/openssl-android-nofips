#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_mark_com_websocket_1openssl_1libuv_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
