workspace(name = "practice")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "com_github_nelhage_rules_boost",
    commit = "df0dd370588b2e3b2307cc38872970397e76757b",
    remote = "https://github.com/zaucy/rules_boost",
    shallow_since = "1594588553 -0700",
)

http_archive(
    name = "boost",
    build_file = "@com_github_nelhage_rules_boost//:BUILD.boost",
    # These patch_cmds do not work on windows without bash
    # patch_cmds = ["rm -f doc/pdf/BUILD"],
    sha256 = "4eb3b8d442b426dc35346235c8733b5ae35ba431690e38c6a8263dce9fcbb402",
    strip_prefix = "boost_1_73_0",
    urls = [
        "https://mirror.bazel.build/dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0.tar.bz2",
        "https://dl.bintray.com/boostorg/release/1.73.0/source/boost_1_73_0.tar.bz2",
    ],
)

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")
boost_deps()