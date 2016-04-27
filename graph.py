import matplotlib.pyplot as plt
size = int(raw_input())
x=[[] for i in range (size)]
y=[[] for i in range (size)]
for i in range(size):
    amt = int(raw_input())
    for j in range(amt):
        a = map(float,raw_input().split())
        x[i].append(a[0])
        y[i].append(a[1])
#plt.subplots(size,1,sharex=True)
plt.figure(1)
plt.title('persistence')
for i in range(size):
    #plt.xlim(20)
    q = plt.subplot(size,1,i+1)
    for j in range(len(x[i])):
        print('Plotting {0} to {1}',x[i][j],y[i][j])
        plt.axhline(y=j+1, xmin=x[i][j], xmax=y[i][j])
    q.set_autoscaley_on(False)
plt.show()
print('Done')