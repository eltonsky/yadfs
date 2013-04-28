#ifndef NAMENODEINSTRUMENTATION_H
#define NAMENODEINSTRUMENTATION_H


class NameNodeInstrumentation
{
    public:
        NameNodeInstrumentation();
        virtual ~NameNodeInstrumentation();

        void incrNumFilesCreated();
        void incrNumCreateFileOps();

    protected:
    private:
};

#endif // NAMENODEINSTRUMENTATION_H
