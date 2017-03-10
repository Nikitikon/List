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
                int*     offset;
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
		int		  findSizeInSegment(Segment *temp, int size);
		void	  defrag(Segment *defragSegment);

        int       segment_size;

        Segment*  current;
};
