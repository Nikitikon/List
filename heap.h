#define SEGMENTSIZE 65536
#define SEGMENTCOUNT 1024

class Heap
{
public:
        Heap(int _segment_size = SEGMENTSIZE)
               { 
                    segment_size  = _segment_size;
                    current       = 0;
               }
        ~Heap()    
               {  delete_segments(); }
        void*      get_mem(int size);
        void       free_mem (void*);
private:
        struct Segment_def
        {       
                bool      used;
                int       size;
                void*     offset;
        };

        struct Segment
        {
                void*        data;
                Segment*     prev;
                Segment_def  descriptor[SEGMENTCOUNT];
                int          descriptor_count;
        };

        int       make_segment();
        void      delete_segments();

        int       segment_size;

        Segment*  current;
};
