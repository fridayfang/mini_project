#ifndef UTILS_DEFINE_H
#define UTILS_DEFINE_H

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&) = delete; \
        TypeName& operator=(const TypeName&) = delete
#endif

#endif  // BAIDU_AD2_SIM_FRAMEWORK_INLUCDE_GLOBAL_DEFINE_H