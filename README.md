# Introduction
VMTHooking is about replacing the virtual functions inside our class. The reason this is possible is because of the rtti table, it allows us to call an function through rtti[0]. We can exploit it by replacing the rtti[0] with our custom function and then execute the original function to keep the flow of the executable program continuning.
