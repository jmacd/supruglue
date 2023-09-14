To test the PRU build w/ TI's compiler:

```
bazel test --config=clpru ...
```

To test using a host toolchain:

```
bazel test --config=testpru ...
```
