load("@rules_proto//proto:defs.bzl", "proto_library")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_proto_library")
load("@com_github_grpc_grpc//bazel:cc_grpc_library.bzl", "cc_grpc_library")

proto_library(
    name = "speechly_proto",
    srcs = ["identity.proto", "slu.proto"],
)

cc_proto_library(
    name = "speechly_cc_proto",
    deps = [":speechly_proto"],
)

cc_grpc_library(
    name = "speechly_cc_grpc",
    srcs = [":speechly_proto"],
    grpc_only = True,
    deps = [":speechly_cc_proto"],
)

cc_library(
    name = "client",
    srcs = ["SpeechlyClient.cc", "Microphone.cc", "IdentityClient.cc", "SluClient.cc"],
    hdrs = ["SpeechlyClient.h", "Microphone.h", "IdentityClient.h", "SluClient.h", "RootCerts.h"],
    linkopts = [
        "-framework CoreFoundation",
        "-framework AudioToolbox",
        "-framework CoreAudio"
    ],
    deps = [
        ":speechly_cc_grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "cpp-example-app",
    srcs = [
        "main.cc"
    ],
    defines = ["BAZEL_BUILD"],
    deps = [
        ":speechly_cc_grpc",
        ":client",
        # http_archive made this label available for binding
        "@com_github_grpc_grpc//:grpc++",
    ],
)
